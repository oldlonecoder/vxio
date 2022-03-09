//
// Created by oldlonecoder on 22-01-31.
//

/**********************************************************************************************************************************
 C opyright 2021 Serge Lussier (oldlonecoder), lussier.serge@gmail.com                                                              *
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *************************************************************************************************************************************/

#include <vxio/interpret/compiler/axt.h>
#include <vxio/util/logger.h>
#include <vxio/util/geometry.h>


using namespace vxio;


std::vector<axt> axt::axt_pool; // A la fin de l'axt on ne fait qu'un axt::axt_pool.clear(); ! :)

axt::axt(token_data* token):t0(token){}
axt::~axt()
{
    logger::debug(src_location) << " :) " ;
}

void* axt::operator new (size_t)
{
    logger::debug(src_location);
    axt::axt_pool.emplace_back(axt());
    return &(*(--axt::axt_pool.end()));
}


std::stack<axt*> axt::pars;





axt::input_table_t axt::input_tbl =
{
    {{type::binary_t,     type::open_pair_t},  &axt::to_right},
    {{type::open_pair_t,  type::leaf_t},       &axt::to_left},
    {{type::open_pair_t,  type::prefix_t},     &axt::to_left},
    {{type::open_pair_t,  type::binary_t},     &axt::to_left},
    {{type::prefix_t,     type::open_pair_t},  &axt::to_right},
    {{type::close_pair_t, type::leaf_t},       &axt::rpar_input_leaf},
    {{type::close_pair_t, type::binary_t},     &axt::close_par},
    {{type::close_pair_t, type::postfix_t},    &axt::close_par},
    {{type::close_pair_t, type::close_pair_t}, &axt::rpar_rpar},
    {{type::prefix_t,     type::close_pair_t}, &axt::input_rpar},
    {{type::leaf_t,       type::close_pair_t}, &axt::input_rpar},
    {{type::leaf_t,       type::postfix_t},    &axt::to_right_to_op},
    {{type::leaf_t,       type::assign_t},     &axt::input_binary},
    {{type::leaf_t,       type::binary_t},     &axt::input_binary},
    {{type::postfix_t,    type::close_pair_t}, &axt::input_rpar},
    {{type::open_pair_t,  type::binary_t},     &axt::to_left},
    {{type::binary_t,     type::binary_t},     &axt::input_binary},
    {{type::binary_t,     type::leaf_t},       &axt::to_right},
    {{type::prefix_t,     type::binary_t},     &axt::input_binary},
    {{type::binary_t,     type::prefix_t},     &axt::to_right},
    {{type::prefix_t,     type::leaf_t},       &axt::to_right},
    {{type::prefix_t,     type::number_t},     &axt::to_right},
    {{type::sign_t,       type::id_t},         &axt::to_right},
    {{type::sign_t,       type::number_t},     &axt::to_right},
    {{type::sign_t,       type::leaf_t},       &axt::to_right},
    {{type::postfix_t,    type::binary_t},     &axt::input_binary},
    {{type::assign_t,     type::binary_t},    &axt::to_right},
    {{type::assign_t,     type::leaf_t},      &axt::to_right},
    {{type::assign_t,     type::prefix_t},    &axt::to_right},
    {{type::assign_t,     type::postfix_t},   &axt::to_right}

};


auto axt::query(axt* lhs, axt* rhs)
{
    for( auto [lr,fn] : axt::input_tbl)
    {
        auto [l,r] = lr;
        if((lhs->t0->s & l) && (rhs->t0->s & r)) return fn;
    }
    return static_cast<axt*(axt::*)(axt*)> (nullptr);
}

axt* axt::input(token_data* token)
{
    for( auto [lr,fn] : axt::input_tbl)
    {
        auto [l,r] = lr;
        if((t0->s & l) && (token->s & r))
        {
            logger::debug(src_funcname) << color::Yellow << t0->text() << ' ' << color::Yellow << vxio::type::name(l) << " <- " << color::Yellow << token->text() << ' ' << color::Yellow << vxio::type::name(r);
            return (this->*fn)(new axt(token));
        }
    }
    logger::error() << " unexpected token:\n" << token->mark();
    return nullptr;
}

axt* axt::begin(token_data* token)
{
    axt* a = new axt(token);
    if(a->t0->c == mnemonic::k_open_par) push_par(a);
    return a;
}

axt* axt::close_par(axt* a)
{
    logger::debug(src_funcname)
    << color::Yellow << t0->text()
    << color::White << ":"
    << color::Lime << rem::code::_function_
    << color::White << ":"
    << color::Yellow << a->t0->text();    // Binary input left par -> tree_set_right is called directly.

    axt* v = lhs;

    // Collapse lhs

    v->op = op;
    if (op) op->rhs = v;

    // discard();

    if (v->op) {
        logger::debug() << color::Yellow << v->op->attribute() << color::CornflowerBlue << " <-- " << color::Yellow <<  a->attribute();
        auto p_fn = query(v->op, a);
        if (!p_fn)
        {
            logger::error() << "syntax error on:\n" << a->t0->mark();
            return nullptr;
        }
        return (v->op->*p_fn)(a);
    }

    v->op = a;
    a->lhs = v;
    return a;
}

axt* axt::input_binary(axt* a)
{
    logger::debug()
    << color::Yellow << t0->text()
    << color::White << "::"
    << color::Yellow << a->t0->text();
    if(t0->is_leaf())
    {
        if(a->t0->c == mnemonic::k_open_par)
        {
            logger::error() <<
            " syntax error in arithmetic expression input : unexpected binary operator token " << a->t0->text() << rem::code::endl << a->t0->mark();
            return nullptr;
        }
    }

    if (t0->c == mnemonic::k_open_par)
        return to_left(a);

    if (t0->is_binary())
    {
        if (!rhs)
        {
            logger::error() <<
            " syntax error in arithmetic expression input : unexpected binary operator token " << a->t0->text() << rem::code::endl <<  a->t0->mark();
            return nullptr;
        }

        if(a->t0->d < t0->d)
            return to_right(a);
        if (op)
            goto op_input_binary;

        return a->to_left(this);
    }

    if (op) {
        op_input_binary:
        auto fn = query(this, a);
        if(fn)
            return (op->*fn)(a);

    }
    a->to_left(this);
    return a;
}

axt* axt::input_leaf(axt* a)
{
    logger::debug()
    << color::Yellow << t0->text()
    << color::White << ":"
    << color::White << ":"
    << color::Yellow << a->t0->text();

    if (!t0->is_operator())
    {
        logger::error(src_location) <<
        "unexpected a right value operand.\n" <<  a->t0->mark();
        return nullptr;
    }
    if (t0->is_binary()) {
        if (!lhs)
        {
            logger::error(src_location) <<
            " binary operator has no left hand side operand." << t0->text() << t0->mark();
            return nullptr;
        }
    }

    ///@Todo Reach interpreter :: mark (token_t*, or xio*  );
    return to_right(a);
}

axt* axt::to_left(axt* a)
{
    /*
     * (;[;{   // Appliqué sur aucun autre type de token car l'appel de tree_set_left ne se fait qu'� partir de tree_input qui r�soud l'associativit�.
     *  /
     * x <- next_token
     * /
     l h*s
     */

    if (lhs) {
        // here we are supposed to be the openning par/index/bracket. so the interior will become right hand side of the parent op of this.
        lhs->op = a;
        a->lhs = lhs;
    }
    a->op = this;

    lhs = a;
    logger::debug()
    << color::White << lhs->t0->details() << color::CornflowerBlue << " <--- " << color::White << t0->details();
    return a;
}

axt* axt::to_right(axt* a)
{
    logger::debug(src_funcname)
    << color::Yellow << t0->text()
    << color::White << " <- "
    << color::Yellow << a->t0->text();

    // Temporary hack....
    if (a->t0->c == mnemonic::k_open_par)
        push_par(a);

    if (rhs) {
        /*
         t h*is
         \
         x <- next_token
         /
         rhs
         */
        logger::debug() << t0->text() << " -> " << rhs->t0->text()
        << color::Lime << "tree_set_right "
        << color::White << " <- "
        << color::Yellow << a->t0->text();
        rhs->op = a;
        a->lhs = rhs;
    }
    rhs = a;
    a->op = this;
    if (t0->is_binary())
    {
       ;// logger::debug(src_funcname) << xio::trace_connect_binary_operands(this);
    }
    return a;
}

axt* axt::to_right_to_op(axt* a)
{
    logger::debug(src_funcname) << color::Yellow << t0->text()
    << color::White << ":"
    << color::Lime << rem::code::_function_
    << color::White << ":"
    << color::Yellow << a->t0->text();

    if (!op) {
        a->lhs = this;
        op = a;
        return a ;
    }
    return op->to_right(a);
}




axt* axt::lpar_input_binary(axt* a)
{
    if (!lhs)
    {
        logger::error(src_funcname) << rem::code::_function_ << " syntax error : \n" << t0->mark();
        return nullptr;
    }
    return to_left(a);
}

axt* axt::input_rpar(axt* a)
{
    logger::debug(src_funcname)
    << color::Lime << rem::code::_function_
    << color::Yellow << t0->text()
    << color::White << " <- "
    << color::Yellow << a->t0->text();

    axt* x = axt::pop_par();
    if (!x)
    {
        logger::error() <<  "Unmatched left paranthese." <<  a->t0->mark();
        return nullptr;
    }
    a->op = x->op;
    a->lhs = x->lhs;
    if(a->lhs)
    {
        if(a->lhs->op)
            a->lhs->op = a;
    }
    if(a->op)
        a->op->rhs = a; // oops!!

    logger::debug(src_funcname) << "new input vertex:[" << color::Yellow << a->t0->text() << color::Reset << "]\n" << a->t0->mark();
    return a;

}

axt* axt::input_lpar(axt* a)
{
    logger::debug(src_funcname)
    << color::Yellow << t0->text()
    << color::White << ":"
    << color::Lime << rem::code::_function_
    << color::White << ":"
    << color::Yellow << a->t0->text();

    // Binary input left par -> tree_set_right is called directly.
    return to_right(a);
}


axt* axt::rpar_input_leaf(axt* a)
{

    logger::debug(src_funcname) << color::Yellow << t0->text()
    << color::White << ":"
    << color::Lime << rem::code::_function_
    << color::White << ":"
    << color::Yellow << a->t0->text();    // Binary input left par -> tree_set_right is called directly.

    if (lhs) {
        logger::debug() << "lhs:" << color::Yellow << lhs->t0->text()
        << color::Reset;

        if (lhs->t0->is_prefix()) {
            if (op) {
                logger::debug() << color::Yellow << lhs->t0->text() << color::White << "'s operator:`" << color::Yellow << lhs->op->t0->text() << color::Reset << "':";
                //op->tree_set_right(lhs);
                lhs->op = op;
                op->rhs = lhs;

            }
            axt* xx = lhs;
            // discard();

            return xx->to_right(a);
        }
    }

    logger::error() << "illegal rvalue token :\n" << a->t0->mark();
    return nullptr;
}

axt* axt::rpar_rpar(axt* a)
{

    logger::debug(src_location) << color::Yellow << t0->text()
    << color::White << ":"
    << color::Lime << rem::code::_function_
    << color::White << ":"
    << color::Yellow << a->t0->text();    // Binary input left par -> tree_set_right is called directly.

    // Collapse lhs
    if (!lhs)
    {
        logger::error() << " syntax error on (function call or empty '( )' not imlemented in expression, yet!) " << attribute() << ":\n" << t0->mark();
        return nullptr;
    }

    axt* v = lhs;

    // Collapse lhs

    v->op = op;
    if (op) op->rhs = v;

    // discard();

    if (v->op) {
        logger::debug()
        << color::Yellow << v->op->attribute() << color::CornflowerBlue
        << " <-- "
        << color::Yellow << a->attribute();

        return v->op->input_rpar(a);
    }

    v->op = a;
    a->lhs = v;
    return a;
}

int axt::push_par(axt* a)
{
    axt::pars.push(a);
    return axt::pars.size();
}

axt* axt::pop_par()
{
    if(axt::pars.empty()) return nullptr;
    axt* a = axt::pars.top();
    return a;
}



void axt::dot_tree_start(iostr& a_out, const iostr& Title)
{
    a_out << "digraph arithmetic_expression_tree {\n";
    a_out << "ratio=compress; ranksep=.55; size = \"6.5,6.5\";\n";
    a_out << "    node [fontname=\"Source Code Pro\", fontsize=12];\n";
    a_out << "    label=\"Arithmetix Expr Tree:[" << Title << "]\"; fontsize = 10;\n";
}

void axt::dot_node(axt* A, iostr& a_out)
{
    //static int nullcount = 0;
    if (!A) return;
    if ((!A->lhs) && (!A->rhs)) return;

    if (A->lhs) {
        a_out << "    A" << A << " -> A" << A->lhs << ";\n";
        axt::dot_node(A->lhs, a_out);
    }
    //     else
    //         t_node::trace_null_node(A, nullcount++, a_stream);

    if (A->rhs) {
        a_out << "    A" << A << " -> A" << A->rhs << ";\n";
        axt::dot_node(A->rhs, a_out);
    }
    //     else
    //         trace_null_node(A, nullcount++, a_stream);
}

void axt::dot_null_node(axt*, int, iostr&)
{

}

void axt::dot_tree(axt* a_root, iostr& a_out)
{
    a_root->dot_attr(a_out);
    axt::dot_node(a_root, a_out);
}

void axt::dot_tree_close(iostr& a_out) { a_out << "}\n"; }


void axt::dot_attr(iostr& a_out)
{
    iostr attr;
    attr << t0->text();
    iostr Shape;
    if (t0->t & vxio::type::text_t)
        Shape << "none";
    else
        if (t0->t & vxio::type::assign_t)
            Shape << "none";
        else
            Shape << "none";

        a_out << "    A" << this << " [shape=%s, label=\"%s\"]\n";
    a_out << Shape << attr;

    if (lhs)
        lhs->dot_attr(a_out);
    if (rhs)
        rhs->dot_attr(a_out);

}


std::string axt::trace_connect_binary_operands(axt* x)
{
    // assume this binary operator already has its lhs rhs operands !!
    //iostr str;
    int lw = x->lhs->attribute().length();
    int rw = x->rhs->attribute().length();
    int ow = x->attribute().length();
    int w = lw + rw + 3; // total width
    w -= lw % 2 == 0;


    int m_lhs = lw - (lw > 1 ? 1 : 0);

    vxy oper_xy = vxy( m_lhs+1,0 );
    oper_xy.x -= ow % 2 == 0 ? 1 : 0;

    winbuffer area;
    area.set_geometry(w, 3); // pour l'instant m'est hardcoded.
    area.gotoxy(oper_xy.x,0);
    area << x->attribute();
    area << vxy{m_lhs,1} << "/ \\";

    area.gotoxy(0, 2);
    area << x->lhs->attribute();

    area.gotoxy(m_lhs+2+(rw<=1?1:0), 2);
    area << x->rhs->attribute();
    return area;
}


void axt::clear_static_pool()
{
    logger::debug(src_funcname) << ":";
    axt::axt_pool.clear();
}
