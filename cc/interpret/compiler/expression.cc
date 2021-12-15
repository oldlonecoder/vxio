//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/compiler/expression.h>
#include <vxio/util/logger.h>
#include <vxio/util/geometry.h>

namespace vxio
{

#define input_trace_log\
       logger::debug(src_funcname) \
        << color::White << type::name(token->t) << ':'\
        << color::Yellow << token->text()\
        << color::White << "<-"\
        << color::Yellow << n->token->text() << ':'\
        << color::White <<  mnemonic_name(n->token->c);

std::stack<expression::node*> pars;
std::stack<expression::node*> indexes;
std::stack<expression::node*> braces;


expression::node::lr_pair_table  expression::node::lr_input_table =
{
     {{type::function_t, type::open_pair_t}, &expression::node::input_fncall_open_pair}, // standard function call : function_id(...)
     {{type::open_pair_t, type::function_t}, &expression::node::input_fncall_open_pair}, // objective-c function call : [function_id] - or - [fncall(|[...)|],...] ... :)
     {{type::id_t, type::open_pair_t}, &expression::node::input_id_open_pair}, // (,[,{,|abs|
     {{type::leaf_t, type::open_pair_t}, &expression::node::input_leaf_open_pair}, // (,[,{,|abs|
     {{type::prefix_t, type::open_pair_t}, &expression::node::input_prefix_open_pair}, // (,[,{,|abs|
     
     {{type::binary_t, type::open_pair_t}, &expression::node::set_right}, // (,[,{,|abs|
     {{type::leaf_t, type::binary_t}, &expression::node::input_leaf_binary_op},
     {{type::binary_t, type::binary_t}, &expression::node::op_input_binary_op},
     {{type::binary_t, type::leaf_t}, &expression::node::input_binary_leaf},
     {{type::postfix_t, type::binary_t}, &expression::node::input_postfix_binary},
     {{type::prefix_t, type::prefix_t}, &expression::node::input_prefix_prefix},
     {{type::open_pair_t, type::leaf_t}, &expression::node::input_open_pair_leaf},
     {{type::open_pair_t, type::operator_t}, &expression::node::input_open_pair_operator},
     
};

expression::node::result expression::node::begin_ast(token_data &token_)
{
    node* n = new expression::node;
    n->token = &token_;
    return n;
}

expression::node::result expression::node::close_ast(token_data &token)
{
        logger::debug(src_funcname)
        << color::White << "::"
        << color::Yellow << token.text();
        
    return logger::warning(src_funcname) << rem::code::_fn_ << ": implement";
}

expression::node::result expression::node::input(token_data &token_)
{
        logger::debug(src_funcname)
        << color::Yellow << token->text()
        << color::White << ":<-:"
        << color::Yellow << token_.text();
        
        
    for(auto [lr,fnptr] : expression::node::lr_input_table)
    {
        auto [l,r] = lr;
        if((token_.s & r) && (this->token->s & l))
        {
            if(fnptr)
            {
                node * new_node = new node;
                new_node->token = &token_;
                return (this->*fnptr)(new_node);
            }
        }
    }
    return logger::syntax() << " unexpected input token:\n" << token_.mark();
}

expression::~expression()
{

}

rem::code expression::parse(context ctx_)
{
    
    _ctx = std::move(ctx_);
    std::cout << "debug (check _ctx): " << _ctx.describe() << "\n";
    
    //Rassembler les tokens de l'expression arithm&eacute;tique:
    
    std::cout << __PRETTY_FUNCTION__ << ": //Rassembler les tokens de l'expression arithm&eacute;tique:\n";
    auto i = _ctx.start;
    while((i <= _ctx.last) && i->_flags.V)
        _ctx.i_tokens.push_back(i++);
    
    if(_ctx.i_tokens.empty())
    {
        logger::error() << ": expression parser: no tokens";
        return rem::code::empty;
    }
    
    
    logger::info() << "maintenant on peut parcourir les tokens et construire l'arbre binaire de l'expression:\n";
    for(auto& tok : _ctx.i_tokens)
        logger::output() << tok->details();
    logger::output() << " ----------------------------------------------------------------\n";
    
    node      *input = nullptr;
    node::result r;
    
    for(auto input_token: _ctx.i_tokens)
    {
        if(!input)
        {
            r = node::begin_ast(*input_token);
            input = *r;
            continue;
        }
        r = input->input(*input_token);
        if(!r)
        {
            return r().rem_code();
        }
        input = *r;
    }
    
    
    _ctx.finish = _ctx.last;
    
    return rem::code::accepted;
}

xio *expression::create_xio(token_data *token_)
{
    
    switch(token_->t)
    {
        //...
        default:return new xio(_ctx.blk, token_, nullptr);
    }
    return nullptr;
}



// -----------------------------------------------input ast ---------------------------------------------------------------------------


 /*
 *       a + b * c
 *
 *           +   <- *     +
 *          / \          / \
 *         a   b        a   * <- c
 *                         /
 *                        b
 *
 */

expression::node::result expression::node::op_input_binary_op(expression::node *n)
{
    input_trace_log
    
    if(token->c == mnemonic::k_open_par)
    {
        return set_left(n);
    }
    if(token->d < n->token->d)
    {
        if(op)
            return op->op_input_binary_op(n);
        return n->set_left(this);
    }
    return set_right(n);
}


/*!
 * @brief leaf operand inputs a binary operator.
 * @param n
 * @return node
 *
 *       a + b * c
 *
 *
 *           +           +             +
 *          / \         / \           / \
 *         a   b <- *  a   * <- c    a   *
 *                        /             / \
 *                       b             b   c
 *
 */
expression::node::result expression::node::input_leaf_binary_op(expression::node *n)
{
   input_trace_log
    if(op)
        return op->op_input_binary_op(n);
    
    n->set_left(this);
    return n;
}


/*!
 * @brief
 * @param n
 * @return
 *
 *       4 a[ a( a{
 *        4(
 *
 */
expression::node::result expression::node::input_id_open_pair(expression::node *n)
{
    input_trace_log
    
    return logger::syntax(src_funcname) << rem::code::_fn_ << ": unexpected token on expression ast node input: unimplemented yet: \n" << n->token->mark();
}

expression::node::result expression::node::input_leaf_open_pair(expression::node *n)
{
        logger::debug(src_funcname)
        << color::Yellow << token->text()
        << color::White << "<-"
        << color::Yellow << n->token->text() << ':'
        << color::White <<  mnemonic_name(n->token->c);
    return logger::syntax(src_funcname) << rem::code::_fn_ << ": unexpected token on expression ast node input: unimplemented yet: \n" << n->token->mark();
}


/*!
 * @brief
 * @param n
 * @return
 *
 *      !a(
 *
 *         !  <- (
 *          \
 *           a
 */
expression::node::result expression::node::input_prefix_open_pair(expression::node *n)
{
    input_trace_log
    return set_right(n);
}

// !*dd
expression::node::result expression::node::input_fncall_open_pair(expression::node *n)
{
    input_trace_log
        
    return logger::warning(src_funcname) << rem::code::_fn_ << ": implement";
}



/*!
 * @brief  set_left
 * @param n
 * @return
 *
 *      !r +
 *      !         +
 *       \       /
 *        r     !
 *               \
 *                r
 */
expression::node::result expression::node::set_left(expression::node *n)
{
    input_trace_log

    n->op = this;
    ls = n;
    logger::debug() << '\n' << node::trace_connect_binary_operands(this);
    return n;
}


//  a * r^2
//     *  <- ^      *
//    / \          / \
//   g   h        g   ^  <- 2
//                   / \
//                  h   2

expression::node::result expression::node::set_right(expression::node *n)
{
    input_trace_log
    if(rs)
    {
        rs->op = n;
        n->ls  = rs;
    }
    rs = n;
    rs->op = this;
    if(n->token->c == mnemonic::k_open_par)
        pars.push(n);
    if(n->token->c == mnemonic::Openindex)
        indexes.push(n);
    if(n->token->c == mnemonic::Openbrace)
        braces.push(n);
    
    logger::debug() << '\n' << node::trace_connect_binary_operands(this);
    return rs;
}

/*!
 * @brief
 * @param n
 * @return
 *
 *   a * ( 1 + 2 );
 *
 *            *
 *           / \
 *          a   +
 *             / \
 *            1   2
 */
expression::node::result expression::node::input_binary_open_pair(expression::node *n)
{
    input_trace_log
    return set_right(n);
}
expression::node::~node()
{
    if(ls)
        delete ls;
    if(rs)
        delete rs;
    
}
expression::node::result expression::node::input_binary_leaf(expression::node *n)
{
    input_trace_log
    if(rs)
        return logger::syntax() << " expected binary or postfix operator:\n" << n->token->mark();
    return set_right(n);
}



/*!
 * @brief input_postix_binary
 * @param n
 * @return
 *
 *     r! + 4;
 *
 *         +
 *        / \
 *       !   4
 *      /
 *     r
 */
expression::node::result expression::node::input_postfix_binary(expression::node *n)
{
    input_trace_log
    return n->set_left(this);
}

/*!
 * @brief input_postfix_postfix
 * @param n
 * @return
 *
 *  !<-r;
 */
expression::node::result expression::node::input_prefix_prefix(expression::node *n)
{
    input_trace_log
    return set_right(n);
}

/*!
 * @brief input_open_pair_leaf
 * @param n
 * @return node*
 *
 * @code
 * 4ac(8 + 5/6)
 *         *                   *                  *                    *
 *        / \                 / \                / \                  / \
 *       4   *               4   *              4   *                4   *
 *          / \                 / \                / \                  / \
 *         a   c <- *          a   * <- (         a   *                a   *
 *                                                   / \                  / \
 *                                                  c   ( <- 8           c   (
 *                                                                          /
 *                                                                         8  <- +
 *
 *        *                         *                   *                        *
         / \                       / \                 / \                      / \
        4   *                     4   *               4   *                    4   *
           / \                       / \                 / \                      / \
          a   *                     a   *               a   *                    a   *
             / \                       / \                 / \                      / \
            c   (                     c   (               c   (                    c   ) <- +  .....
               /                         /                   /                        /
              +                         +                   +                        +
             /                         / \                 / \                      / \
            8 <- 5                    8   5 <-/           8  [/]                   8  [/]
 *                                                           / \                      / \
 *                                                          5   6 <- )               5   6
 * @endcode
 */
expression::node::result expression::node::input_open_pair_leaf(expression::node *n)
{
    set_left(n);
    return n;
}

expression::node::result expression::node::input_open_pair_operator(expression::node *n)
{
    n->op = this;
    ls->op = n;
    if(n->token->is_prefix())
        n->rs = ls;
    else
        n->ls = ls;
    return n;
}



// <summary>
///
///          +
///         / \
///        va vari
/// </summary>
/// <returns></returns>
 std::string expression::node::trace_connect_binary_operands(expression::node* x)
 {
     // assume this binary operator already has its lhs rhs operands !!
     //iostr str;
     int lw,rw,ow,w = 0;
     if(x->ls) lw = x->ls->token->text().length();
     if(x->rs) rw = x->rs->token->text().length();
     ow = x->token->text().length();
     w = lw + rw + 3; // total width
     w -= lw % 2 == 0;
     
     int m_lhs = lw - (lw > 1 ? 1 : 0);

     vxy oper_xy = vxy( m_lhs+1,0 );
     oper_xy.x -= ow % 2 == 0 ? 1 : 0;

     winbuffer area;
     area.set_geometry(w, 3); // pour l'instant m'est hardcoded.
     area.gotoxy(oper_xy.x,0);
     area << x->token->text();
     area << vxy{m_lhs,1} << "/ \\";
    
     if(x->ls)
     {
         area.gotoxy(0, 2);
         area << x->ls->token->text();
     }
     if(x->rs)
     {
         area.gotoxy(m_lhs + 2 + (rw <= 1 ? 1 : 0), 2);
         area << x->rs->token->text();
     }
     return area;
 }


}