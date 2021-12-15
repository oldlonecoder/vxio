//
// Created by slussier on 19-08-25.
//

#include <vxio/interpret/xio.h>
#include <vxio/util/logger.h>
#include <vxio/util/geometry.h>
#include <cmath>



namespace vxio {

    xio* xio::_global_root_bloc = nullptr;


    //xio* xio::get_global_root() { return xio::{vxio::type::binary_t,     vxio::type::open_pair_t},  &xio::set_right_global_root_bloc; }


#define deg2rad(_xio) (((_xio)->acc->v.d*3.1415) / 180)
    constexpr auto _PI_ = 3.14159265358979323846;
    // M_PI ?
    std::stack<xio*> xio::pars;
    std::stack<xio*> xio::indexes;
    std::stack<xio*> xio::curls;


    // void xio::// discard()
    // {
    //     for (auto x : xio::xio_gc)
    //         if (this == x) return;

    //     xio::xio_gc.push_back(this);
    //     lhs = rhs = op = nullptr; // Make sure we do not accidentally reuse this;
    // }

    using token_t = token_data;

    // Arithmetic Binary Tree: associative building logic table:
    xio::lr_input_table      xio::lr_table = {

        {{vxio::type::binary_t,     vxio::type::open_pair_t},  &xio::tree_set_right},
        {{vxio::type::open_pair_t,  vxio::type::leaf_t},       &xio::tree_set_left},
        {{vxio::type::open_pair_t,  vxio::type::prefix_t},     &xio::tree_set_left},
        {{vxio::type::open_pair_t,  vxio::type::binary_t},     &xio::tree_set_left},
        {{vxio::type::prefix_t,     vxio::type::open_pair_t},  &xio::tree_set_right},
        {{vxio::type::close_pair_t, vxio::type::leaf_t},       &xio::tree_rpar_input_leaf},
        {{vxio::type::close_pair_t, vxio::type::binary_t},     &xio::tree_close_par},
        {{vxio::type::close_pair_t, vxio::type::postfix_t},    &xio::tree_close_par},
        //{{vxio::type::close_pair_t, vxio::type::close_pair_t}, &xio::tree_close_par},
        {{vxio::type::close_pair_t, vxio::type::close_pair_t}, &xio::tree_rpar_rpar},
        {{vxio::type::prefix_t,     vxio::type::close_pair_t}, &xio::tree_input_rpar},
        {{vxio::type::leaf_t,       vxio::type::close_pair_t}, &xio::tree_input_rpar},
        {{vxio::type::leaf_t,       vxio::type::postfix_t},    &xio::tree_set_right_to_op},
        {{vxio::type::leaf_t,       vxio::type::assign_t},     &xio::tree_input_binary},
        {{vxio::type::leaf_t,       vxio::type::binary_t},     &xio::tree_input_binary},
        {{vxio::type::postfix_t,    vxio::type::close_pair_t}, &xio::tree_input_rpar},
        {{vxio::type::open_pair_t,  vxio::type::binary_t},     &xio::tree_set_left},
        {{vxio::type::binary_t,     vxio::type::binary_t},     &xio::tree_input_binary},
        {{vxio::type::binary_t,     vxio::type::leaf_t},       &xio::tree_set_right},
        {{vxio::type::prefix_t,     vxio::type::binary_t},     &xio::tree_input_binary},
        {{vxio::type::binary_t,     vxio::type::prefix_t},     &xio::tree_set_right},
        {{vxio::type::prefix_t,     vxio::type::leaf_t},       &xio::tree_set_right},
        {{vxio::type::prefix_t,     vxio::type::number_t},     &xio::tree_set_right},
        {{vxio::type::sign_t,       vxio::type::id_t},         &xio::tree_set_right},
        {{vxio::type::sign_t,       vxio::type::number_t},     &xio::tree_set_right},
        {{vxio::type::sign_t,      vxio::type::leaf_t},      &xio::tree_set_right},
        {{vxio::type::postfix_t, vxio::type::binary_t},  &xio::tree_input_binary},

        {{vxio::type::assign_t,  vxio::type::binary_t},  &xio::tree_set_right},
        {{vxio::type::assign_t,  vxio::type::leaf_t},    &xio::tree_set_right},
        {{vxio::type::assign_t,  vxio::type::prefix_t},  &xio::tree_set_right},
        {{vxio::type::assign_t,  vxio::type::postfix_t}, &xio::tree_set_right}

    };


    // Primary arithmetic supported operators table:
    xio::xio_opfn_table_t   xio::xio_operators_table{
        {mnemonic::k_left_shift,  &xio::lshift},
        {mnemonic::k_radical,     &xio::radical},
        {mnemonic::k_exponent,    &xio::exponent},
        {mnemonic::k_right_shift, &xio::rshift},
        {mnemonic::k_decr,       &xio::decr},
        {mnemonic::k_incr,              &xio::incr},
        {mnemonic::k_assign_add,         &xio::assignadd},
        {mnemonic::k_assign_sub,         &xio::assignsub},
        {mnemonic::k_assign_mul,         &xio::assignmul},
        {mnemonic::k_assign_div,         &xio::assigndiv},
        {mnemonic::k_assign_mod,         &xio::assignmod},
        {mnemonic::k_assign_and,         &xio::assignand},
        {mnemonic::k_assign_or,          &xio::assignor},
        {mnemonic::k_assign_xor,         &xio::assignxor},
        {mnemonic::k_assign_c1,          &xio::assignx1},
        {mnemonic::k_assign_left_shift,  &xio::assignlshift},
        {mnemonic::k_assign_right_shift, &xio::assignrshift},
        {mnemonic::k_less_eq,      &xio::leq},
        {mnemonic::k_greater_eq,   &xio::geq},
        {mnemonic::k_equal,        &xio::eq},
        {mnemonic::k_not_equal,    &xio::neq},
        {mnemonic::k_add,          &xio::add},
        {mnemonic::k_sub,          &xio::sub},
        {mnemonic::k_mul,          &xio::mul},
        {mnemonic::k_modulo,       &xio::modulo},
        {mnemonic::k_less_than,    &xio::lt},
        {mnemonic::k_greater_than, &xio::gt},
        {mnemonic::k_assign,      &xio::assign},
        {mnemonic::k_binary_and,  &xio::binand},
        {mnemonic::k_binary_or,   &xio::binor},
        {mnemonic::k_binary_xor,  &xio::bitxor},
        {mnemonic::k_c1,          &xio::x1},
        {mnemonic::k_c2,          &xio::x2},
        {mnemonic::k_binary_not,  &xio::bitnot},
        {mnemonic::k_logical_and, &xio::booland},
        {mnemonic::k_logical_or,  &xio::boolor},
        {mnemonic::k_div,         &xio::division},
        {mnemonic::k_factorial,   &xio::factorial},
        {mnemonic::k_positive,    &xio::positive},
        {mnemonic::k_negative,    &xio::negative},
        {mnemonic::k_pi,          &xio::kpi},
        {mnemonic::k_cosinus,     &xio::kcos},
        {mnemonic::k_arc_cosinus, &xio::kacos},
        {mnemonic::k_tangent,     &xio::ktan},
        {mnemonic::k_arc_tangent, &xio::katan},
        {mnemonic::k_sinus,       &xio::ksin},
        {mnemonic::k_arc_sinus,   &xio::kasin},
        {mnemonic::k_number,      &xio::knumber},
        {mnemonic::k_u8,          &xio::ku8   },
        {mnemonic::k_u16,         &xio::ku16  },
        {mnemonic::k_u32,         &xio::ku32  },
        {mnemonic::k_u64,         &xio::ku64  },
        {mnemonic::k_i8,          &xio::ki8   },
        {mnemonic::k_i16,         &xio::ki16  },
        {mnemonic::k_i32,         &xio::ki32  },
        {mnemonic::k_i64,         &xio::ki64  },
        {mnemonic::k_float,       &xio::kreal },
        {mnemonic::k_string, &xio::kstring}
    };


    // xio::xio()
    // {
    //     //acc = std::make_shared<alu>(0.0);
    // }


    xio::xio()
    {
        acc = new alu(0.0f); // Shit!
    }

    xio::xio(xio* a_parent) :_parent(a_parent)
    {
        //token_info = new token_t;
        acc = new alu;
    }

    xio::~xio()
    {
        if (!mem.xshared) delete acc;
    }

    xio::xio(xio* a_parent, token_t* a_token, alu* a_alu) :_parent(a_parent)
    {
        t0 = a_token;
        logger::debug(src_location) << "xio::xio => token details:[" << (t0 ? t0->details() : "nullptr") << "] \n";

        if (a_alu != nullptr)
        {
            acc = a_alu;
            mem.own = 0;
            return;
        }

        acc = new alu(0.0f);

        if (!t0)
            return;

        switch (t0->t) {
        case vxio::type::text_t:
            *acc = a_token->text();
            return;
        case vxio::type::leaf_t:
        {
            ///@todo Attention enlever le court-circuit du type-size!!!
            if(t0->s & vxio::type::number_t)
            {
                double d;
                (iostr(a_token->text())) >> d;
                *acc = d;
                logger::debug() << " acc: " << color::Yellow << (*acc)();
            }
        }
        return;
        case vxio::type::hex_t:
        {
            uint64_t d;
            iostr(a_token->text()).hexadecimal(d);
            *acc = d;
            return;
        }
        return;
        case vxio::type::any_t:
        case vxio::type::void_ptr:
            *acc = (void*)nullptr;
            return;
        default:
            
            break;
        }

        auto i = xio::xio_operators_table.find(a_token->c);
        xio_fn = i != xio::xio_operators_table.end() ? i->second : nullptr;
        logger::debug(src_location) << " acc: '" << color::Yellow << (*acc)() << color::White << "'";
    }

    

    rem::code xio::instanciate_from(xio*)
    {
        return rem::code::implement;
    }

    // -------------- ARITHMETIC BINARY TREE INPUT LOGIC IMPLEMENTATION ------------------------------------
    /*
            a = 32 * (2+3) / 4

                        =           =               =              =              =            =              =               = <---+           =
                       / \         / \             / \            / \            / \          / \            / \             / \    |          / \
                      a  [/]      a   *  <- (     a   *          a   *          a   *        a   *          a   *           a   * <-|         a   /
                         /  \        /               / \            / \            / \          / \            / \             / \  |            / \
                        *    4 ;    32              32  ( <- 2     32  (          32  (        32 ( <--+      32  ) <- /      32  + /           *   4;
                       / \                                            /              /           /     |         /               / \           / \
                      32  +                                          2 <- +         + <- 3      +      |        +               2   3         32  +
                         / \                                                       /           / \     +       / \                               / \
                        2   3                                                     2           2   3 <- )      2   3                             2   3
    */
    xio* xio::tree_input(token_t* a_token, const xio::maker& invoke_maker)
    {
        for (auto [lr_pair, fnptr] : xio::lr_table)
        {
            auto [l, r] = lr_pair;
            auto lr_fnptr = fnptr;
            
            if ((l & t0->s) && (a_token->s & r))
            {
                logger::debug(src_funcname)
                    << color::Blue << "xio::tree_input: \n"
                    << color::Yellow
                    << t0->text() << ' '
                    << color::Yellow << vxio::type::name(l) << " <- "
                    << color::Yellow << a_token->text() << ' '
                    << color::Yellow << vxio::type::name(r);
                
                if(lr_fnptr)
                {
                    if (!invoke_maker)
                    {
                        logger::error() << " arithmetic expression binary tree build: cannot invoke null xio maker!";
                        return nullptr;
                    }
                    xio* x = invoke_maker(a_token);
                    if (!x) return nullptr;
                    logger::debug() << ":\n" << t0->mark();
                    return (this->*lr_fnptr)(x);
                }
            }
        }
        logger::error() << " unexpected token:\n" << a_token->mark();
        return nullptr;
    }


    xio* xio::begin_expr(token_t *a_token, xio::maker invoke_maker)
    {
        if(invoke_maker)
        {
            logger::debug() << " => token infos:" << (a_token ? a_token->details() : "nullptr");
            xio *x = invoke_maker(a_token);
            if(x->t0->c == mnemonic::k_open_par)
                push_lpar(x);
             return x;
        }
        return nullptr;
    }

    void xio::push_lpar(xio* lpar)
    {
        logger::debug(src_location);
        xio::pars.push(lpar);
    }

    xio* xio::pop_lpar()
    {
        if (xio::pars.empty()) return nullptr;
        xio* x = xio::pars.top();
        xio::pars.pop();
        return x;
    }

    // q = 0.4 - |bb|; 
    // 
    // 56==3*2/(4+5)-3
    /*  
          == <- 3     ==            ==           ==               ==                ==           
         /           /  \          /  \         /  \             /  \              /  \          
        56          56   3 <- *   56   * <- 2  56   *           56   * <----+     56   / <- (    
                                      /            / \              / \     |         /          
                                     3            3   2 <- /       3   2 <- /        *           
                                                                                    / \          
                                                                                   3   2         
    ==                  == 
   /  \                /  \
  56   /              56   /
      / \                 / \      
     *   ( <- 4          *   \
    / \                 / \   \ 
   3   2               3   2   (
                              /
                             4 <- + 
                                                                                             
                                                                                                      
                                            
      ==                    ==                   == <------+       ==          
     /  \                  /  \                 /  \       |      /  \        
    56   /                56   / <------+      56   / <----+     56   -             
        / \                   / \       |          / \     |         / \  
       *   \                 *  [\]     |         *   \    |       [/]  3
      / \   \               / \   \     |        / \   \   |       / \    
     3   2   ( <--+        3   2   ) <- -       3   2   - --      *   \ 
            /     |               /                    / \       / \   +
           +      |              +                    +   3     3   2 / \
          / \     |             / \                  / \             4   5
         4   5 <- )            4   5                4   5       

         
         
*/
    xio* xio::tree_input_binary(xio* x) {

        logger::debug() 
            << color::Yellow << t0->text()
            << color::White << ":"
            << color::White << ":"
            << color::Yellow << x->t0->text();
        if(t0->is_leaf())
        {
            if(x->t0->c == mnemonic::k_open_par)
            {
                logger::error() <<
                                " syntax error in arithmetic expression input : unexpected binary operator token " << x->t0->text() << ":\n" << x->t0->mark();
                return nullptr;
            }
        }
        if (t0->c == mnemonic::k_open_par)
            return tree_set_left(x);

        if (t0->is_binary()) 
        {
            if (!rhs) 
            {
                logger::error() <<
                    " syntax error in arithmetic expression input : unexpected binary operator token " << x->t0->text() << ":\n" <<  x->t0->mark();
                return nullptr;
            }
            
            if(x->t0->d < t0->d)
                return tree_set_right(x);
            if (op)
                goto op_input_binary;
            
            return x->tree_set_left(this);
        }

        if (op) {
            op_input_binary:
            lr_input_fnptr ptr = associate(op, x);
            if (ptr)
                return (op->*ptr)(x);

        }
        x->tree_set_left(this);
        return x;
    }


    xio* xio::tree_input_leaf(xio* x) {

        logger::debug()
            << color::Yellow << t0->text()
            << color::White << ":"
            << color::White << ":"
            << color::Yellow << x->t0->text();

        if (!t0->is_operator())
        {
            logger::error(src_location) <<
              "unexpected a right value operand.\n" <<  x->t0->mark();
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
        return tree_set_right(x);
    }

    /*!

        @note Attention: call tree_set_left only when isolating a sub-expressiom such as '('; '['; '{'.
        -( 5! )


              -
               \
                (
               /
              ! <- +
             /
            5
    */
    xio* xio::tree_set_left(xio* x)
    {

        /*
                  (;[;{   // Appliqué sur aucun autre type de token car l'appel de tree_set_left ne se fait qu'� partir de tree_input qui r�soud l'associativit�.
                   /
                  x <- next_token
                 /
               lhs
         */

        if (lhs) {
            // here we are supposed to be the openning par/index/bracket. so the interior will become right hand side of the parent op of this.
            lhs->op = x;
            x->lhs = lhs;
        }
        x->op = this;

        lhs = x;
        logger::debug()
            << color::White << lhs->t0->details() << color::CornflowerBlue << " <--- " << color::White << t0->details();
        return x;
    }


    /*

        /             /
         \             \
          a < (         ( < 2
                       /
                      a
    */
    xio* xio::tree_set_right(xio* x) {

        logger::debug(src_funcname)
            << color::Yellow << t0->text()
            << color::White << " <- "
            << color::Yellow << x->t0->text();

        // Temporary hack....
        if (x->t0->c == mnemonic::k_open_par)
            push_lpar(x);

        if (rhs) {
            /*
               this
                  \
                   x <- next_token
                  /
                rhs
            */
            logger::debug() << t0->text() << " -> " << rhs->t0->text()
                << color::Lime << "tree_set_right "
                << color::White << " <- "
                << color::Yellow << x->t0->text();
            rhs->op = x;
            x->lhs = rhs;
        }
        rhs = x;
        x->op = this;
        if (t0->is_binary())
        {
            logger::debug(src_funcname) << xio::trace_connect_binary_operands(this);
        }
        return x;
    }


    xio* xio::tree_set_right_to_op(xio* x)
    {
        logger::debug(src_funcname) << color::Yellow << t0->text()
            << color::White << ":"
            << color::Lime << rem::code::_function_
            << color::White << ":"
            << color::Yellow << x->t0->text();

        if (!op) {
            x->lhs = this;
            op = x;
            return x ;
        }
        return op->tree_set_right(x);
    }


    xio::lr_input_fnptr xio::associate(xio* a_lhs, xio* a_rhs)
    {
        for (auto key : xio::lr_table) {
            //if ((key.first.first == a_lhs->token_info->t) && (a_rhs->token_info->t & key.first.second)) {
            if ((key.first.first & a_lhs->t0->s) && (a_rhs->t0->s & key.first.second)) {
                return key.second;
            }
        }
        return nullptr;
    }


    xio* xio::tree_close()
    {
        logger::debug(src_funcname) << rem::code::_fn_
        << "closing tree from xio node: " << t0->details() << ":\n" << t0->mark();

        if (t0->c == mnemonic::k_open_par)
        {            
            logger::error() << " unexpected end of file.";
            return nullptr;
        }  

        if (!xio::pars.empty()) 
        {
            xio* x = xio::pars.top();
            xio::pars.pop();
            logger::error() << " umatched closing parenthese from:\n" << x->t0->mark();
            return nullptr;
        }
        

        if (t0->c == mnemonic::k_close_par) {
            logger::debug() << "Closing the tree on close parenthese:";
            if (lhs) 
            {
                xio* x = lhs;
                logger::debug() << "left hand side operand: " << lhs->t0->details() << ":\n" << lhs->t0->mark();

                lhs->op = op;

                if (op) 
                {
                    op->rhs = lhs;
                    xio::trace_connect_binary_operands(op);
                }

                // discard();
                return x->tree_root();
            }
        }
        return tree_root();
    }


    xio* xio::tree_root(bool skip_syntax)
    {
        logger::debug(src_funcname) << rem::code::_fn_
            << "query tree ins from xio node:\n " << t0->mark();
        xio* x = this;
        xio* p = x;
        do {
            x = p;
            if(!skip_syntax)
            {
                switch (x->t0->t) {
                case vxio::type::assign_t:
                case vxio::type::binary_t:
                    if (!x->lhs)
                    {
                       logger::error() << "Syntax error: binary operator is missing its left operand.\n" << x->t0->mark();
                        return nullptr;
                    }
                    if (!x->rhs)
                    {
                        logger::error() << "Syntax error: binary operator is missing its right operand.\n" << x->t0->mark();
                        return nullptr;
                    }
                case vxio::type::prefix_t:
                    if (!x->rhs)
                    {
                        logger::error() << "Syntax error: prefix unary operator is missing its (right) operand.\n" << x->t0->mark();
                        return nullptr;
                    }
                    break;
                case vxio::type::postfix_t:
                    if (!x->lhs)
                    {
                        logger::error() << "Syntax error: postfix unary operator is missing its (left) operand.\n" << x->t0->mark();
                        return nullptr;
                    }
                    break;
                }
            }
            p = p->op;
        } while (p);

        logger::debug(src_location) << rem::code::_fn_ << "query tree ins returning node: " << x->t0->details();
        return  x;
    }

    void xio::detach()
    {
        if (_parent)
            _parent->remove_child(this);
        _parent = nullptr;
    }

    void xio::remove_child(xio* c)
    {
        auto i = _children.begin();
        while (i != _children.end()) 
        {
            if (*i == c) break;
            ++i;
        }
        
        if (i == _children.end())
            return;
        _children.erase(i);
    }

    xio* xio::tree_lpar_input_binary(xio* x)
    {
        if (!lhs) 
        { 
            logger::error(src_funcname) << rem::code::_function_ << " syntax error : \n" << t0->mark();
            return nullptr;
        }
        return tree_set_left(this);
    }


    xio* xio::tree_input_rpar(xio* rpxio)
    {
        logger::debug(src_funcname)
            << color::Lime << rem::code::_function_
            << color::Yellow << t0->text()
            << color::White << " <- "
            << color::Yellow << rpxio->t0->text();

        xio* x = xio::pop_lpar();
        if (!x)
        {
            logger::error() <<  "Unmatched left paranthese." <<  rpxio->t0->mark();
            return nullptr;
        }
        rpxio->op = x->op;
        rpxio->lhs = x->lhs;
        if(rpxio->lhs)
        {
            if(rpxio->lhs->op)
                rpxio->lhs->op = rpxio;
        }
        if(rpxio->op)
           rpxio->op->rhs = rpxio; // oops!!
           
        logger::debug() 
            << "new input vertex:[" 
            << color::Yellow 
            << rpxio->t0->text() 
            << color::Reset 
            << "]\n"
            << rpxio->t0->mark();

        return rpxio;
    }


    xio* xio::tree_input_lpar(xio* x)
    {

        logger::debug(src_funcname)
            << color::Yellow << t0->text()
            << color::White << ":"
            << color::Lime << rem::code::_function_
            << color::White << ":"
            << color::Yellow << x->t0->text();

        // Binary input left par -> tree_set_right is called directly.
        return tree_set_right(x);
    }

    xio* xio::tree_close_par(xio* x)
    {
        logger::debug(src_funcname)
            << color::Yellow << t0->text()
            << color::White << ":"
            << color::Lime << rem::code::_function_
            << color::White << ":"
            << color::Yellow << x->t0->text();    // Binary input left par -> tree_set_right is called directly.

        xio* v = lhs;

        // Collapse lhs

        v->op = op;
        if (op) op->rhs = v;

        // discard();

        if (v->op) {
            logger::debug()
                << color::Yellow << v->op->attribute() << color::CornflowerBlue 
                << " <-- " 
                << color::Yellow <<  x->attribute();

            auto p_fn = associate(v->op, x);
            if (!p_fn)
            { 
                logger::error() << "syntax error on:\n" << x->t0->mark();
                return nullptr;
            }
            return (v->op->*p_fn)(x);
        }

        v->op = x;
        x->lhs = v;
        return x;
    }


    xio* xio::tree_rpar_input_leaf(xio* x)
    {
        logger::debug(src_funcname) << color::Yellow << t0->text()
            << color::White << ":"
            << color::Lime << rem::code::_function_
            << color::White << ":"
            << color::Yellow << x->t0->text();    // Binary input left par -> tree_set_right is called directly.

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
                xio* xx = lhs;
                // discard();

                return xx->tree_set_right(x);
            }
        }
        
        logger::error() << "illegal rvalue token :\n" << x->t0->mark();
        return nullptr;
    }


    xio* xio::tree_rpar_rpar(xio* r)
    {

/*

    (a-(b*m/(d+e)-f))
                    ^
                              
                              
              -               
             / \              
            a   -                  
               / \          
              /               
             / \              
            *   \             
           / \   \            
          b   m   +
                 / \          
                d   e         
*/

        logger::debug(src_location) << color::Yellow << t0->text()
            << color::White << ":"
            << color::Lime << rem::code::_function_
            << color::White << ":"
            << color::Yellow << r->t0->text();    // Binary input left par -> tree_set_right is called directly.

   // Collapse lhs
        if (!lhs)
        {
            logger::error() << " syntax error on (function call or empty '( )' not imlemented in expression, yet!) " << attribute() << ":\n" << t0->mark();
            return nullptr;
        }

        xio* v = lhs;

        // Collapse lhs

        v->op = op;
        if (op) op->rhs = v;

        // discard();

        if (v->op) {
            logger::debug()
                << color::Yellow << v->op->attribute() << color::CornflowerBlue
                << " <-- "
                << color::Yellow << r->attribute();

            return v->op->tree_input_rpar(r);
        }

        v->op = r;
        r->lhs = v;
        return r;
    }


    // ------------------ ARITHMETIC OPERATORS IMPLEMENTATION ---------------------------------------------

    alu xio::jsr()
    {
        //...

        if (lhs) *acc = lhs->jsr(); // Always catch the lhs value so we return that one if there is no rhs operand.
        if (rhs) *acc = rhs->jsr(); // Always catch the rhs value because it is the value to be returned after being applied to the lhs (if applicable).

        if (xio_fn)
            return (this->*xio_fn)();// All operators assign acc.
        else {
            if (t0->is_operator()) {
                logger::warning() << "xio [" << color::Yellow << t0->text() << color::Reset << "] has no rtfn (yet?).:\n" << t0->mark();
            }
        }
        t0->s |= acc->T;
        return *acc;
    }


    alu xio::lshift()
    {
        *acc = lhs->acc->number<uint64_t>() << rhs->acc->number<uint64_t>();
        return *acc;
    }

    alu xio::radical()
    {
        *acc = std::pow(lhs->acc->number<double>(), 1 / rhs->acc->number<double>());
        return *acc;
    }

    alu xio::exponent()
    {
        *acc = std::pow(lhs->acc->number<double>(), rhs->acc->number<double>());
        return *acc;
    }

    alu xio::rshift()
    {
        *acc = lhs->acc->number<uint64_t>() >> rhs->acc->number<uint64_t>();
        return *acc;
    }


    alu xio::decr()
    {
        *acc = t0->is_prefix() ? --(*rhs->acc) : (*lhs->acc)--;
        return *acc;
    }


    alu xio::incr()
    {
        *acc = t0->is_prefix() ? ++(*rhs->acc) : (*lhs->acc)++;
        return *acc;
    }


    alu xio::assignadd()
    {
        *acc = *(lhs->acc) += *(rhs->acc);
        return  *acc;
    }


    alu xio::assignsub()
    {
        *acc = *(lhs->acc) -= *(rhs->acc);
        return  *acc;
    }


    alu xio::assignmul()
    {
        *acc = *(lhs->acc) *= *(rhs->acc);
        return *acc;
    }


    alu xio::assigndiv()
    {
        *acc = *(lhs->acc) /= *(rhs->acc);
        return *acc;
    }
    alu xio::assignmod()
    {
        *acc = *(lhs->acc) %= *(rhs->acc);
        return *acc;

    }
    alu xio::assignand()
    {
        *acc = *(lhs->acc) &= *(rhs->acc);
        return *acc;

    }
    alu xio::assignor()
    {
        *acc = *(lhs->acc) |= *(rhs->acc);
        return *acc;

    }
    alu xio::assignxor()
    {
        *acc = *(lhs->acc) ^= *(rhs->acc);
        return *acc;

    }
    alu xio::assignx1()
    {
        *acc = ~(*rhs->acc);
        return *acc;
    }
    alu xio::assignlshift()
    {
        *acc = *lhs->acc <<= *rhs->acc;
        return *acc;
    }


    alu xio::assignrshift()
    {
        *acc = *lhs->acc >>= *rhs->acc;
        return *acc;
    }


    alu xio::leq()
    {
        *acc = *lhs->acc <= *rhs->acc;
        return *acc;
    }


    alu xio::geq()
    {
        *acc = *lhs->acc >= *rhs->acc;
        return *acc;
    }


    alu xio::eq()
    {
        *acc = *lhs->acc == *rhs->acc;
        return *acc;
    }


    alu xio::neq()
    {
        *acc = *lhs->acc != *rhs->acc;
        return *acc;
    }

    alu xio::add()
    {
        logger::debug(src_location) << color::Lime << rem::code::_function_
            << color::Yellow << lhs->unit()() << " " << color::CornflowerBlue << attribute() << " " << color::Yellow << rhs->unit()() << ":";
        *acc = *lhs->acc + *rhs->acc;
        logger::output() << color::CornflowerBlue << " = " << color::Lime << (*acc)();
        return *acc;
    }
    alu xio::sub()
    {
        // hack... en attendant :
        if(t0->s & vxio::type::sign_t)
            return negative();
        logger::debug(src_location) << color::Lime << rem::code::_function_
            << color::Yellow << lhs->unit()() << " " << color::CornflowerBlue << attribute() << " " << color::Yellow << rhs->unit()() << ":";
        *acc = *lhs->acc - *rhs->acc;
        logger::output() << color::CornflowerBlue << " = " << color::Lime << (*acc)();

        return *acc;
    }
    alu xio::mul()
    {
        logger::debug()
            << color::Yellow << lhs->attribute()
            << color::CornflowerBlue << attribute()
            << color::Yellow << rhs->attribute()
            << color::White << "=";
        *acc = *lhs->acc * *rhs->acc;
        logger::output() << (*acc)();
        return *acc;
    }
    alu xio::modulo()
    {
        *acc = *lhs->acc % *rhs->acc;
        return *acc;
    }
    alu xio::lt()
    {
        *acc = *lhs->acc < *rhs->acc;
        return *acc;
    }
    alu xio::gt()
    {
        *acc = *lhs->acc > *rhs->acc;
        return *acc;
    }
    alu xio::assign()
    {
        logger::debug(src_location) << color::Lime << rem::code::_function_
            << color::Aquamarine3 << lhs->attribute() << color::Reset << " "
            << " " << color::CornflowerBlue << attribute() << " "
            << color::Yellow
            << rhs->unit()() << ":";

        *acc = *lhs->acc = *rhs->acc;
        logger::output() << color::CornflowerBlue << " => " << color::Lime << (*acc)();
        return *acc;
    }
    alu xio::binand()
    {
        *acc = *lhs->acc & *rhs->acc;
        return *acc;
    }
    alu xio::binor()
    {
        *acc = *lhs->acc | *rhs->acc;
        return *acc;
    }
    alu xio::bitxor()
    {
        *acc = *lhs->acc ^ *rhs->acc;
        return *acc;
    }
    alu xio::x1()
    {
        *acc = ~(*lhs->acc);
        return *acc;
    }
    alu xio::x2()
    {
        *acc = ~(*lhs->acc) + alu(1);
        return *acc;
    }


    alu xio::bitnot()
    {
        *acc = !rhs->acc->number<double>();
        return *acc;
    }


    alu xio::booland()
    {
        *acc = *lhs->acc && *rhs->acc;
        return *acc;
    }
    alu xio::boolor()
    {
        *acc = *lhs->acc || *rhs->acc;
        return *acc;

    }

    alu xio::division()
    {
        logger::debug(src_location) << color::Lime << rem::code::_function_
            << color::Yellow << lhs->unit()() 
            << " " << color::CornflowerBlue << attribute() << " " 
            << color::Yellow 
            << rhs->unit()() << ":";

        *acc = *lhs->acc / *rhs->acc;
        return *acc; 
        logger::output() << color::CornflowerBlue << " = " << color::Lime << (*acc)();
    }
    alu xio::factorial()
    {
        *acc = lhs->factorial();
        return *acc;
    }
    alu xio::positive()
    {
        if (rhs->acc->number<double>() < 0)
            *rhs->acc *= -1;

        *acc = *rhs->acc;
        return *acc;
    }


    alu xio::negative()
    {
        logger::debug(src_location) << color::Lime << rem::code::_function_
            << color::CornflowerBlue << attribute()
            << color::Yellow << rhs->attribute()
            << color::White << "=";

        if (rhs->acc->number<double>() > 0) // ==> a = -1;  -a = ?
            *rhs->acc *= -1;

        *acc = *rhs->acc;
        logger::output() << color::CornflowerBlue << " = " << color::Lime << (*acc)();
        return *acc;
    }


    alu xio::kpi()
    {
        *acc = alu(_PI_);
        return *acc;
    }


    alu xio::kcos()
    {
        *acc = std::cos(deg2rad(rhs));
        return *acc;
    }


    alu xio::kacos()
    {
        *acc = std::acos(deg2rad(rhs));
        return *acc;
    }
    alu xio::ktan()
    {
        *acc = std::tan(deg2rad(rhs));
        return *acc;
    }


    alu xio::katan()
    {
        *acc = std::atan(deg2rad(rhs));

        return *acc;
    }

    alu xio::ksin()
    {
        *acc = std::sin(deg2rad(rhs));
        return *acc;
    }

    alu xio::kasin()
    {
        *acc = std::asin(deg2rad(rhs));
        return *acc;
    }

    alu xio::knumber()
    {

        return *acc;
    }

    alu xio::ku8()
    {
        *acc = rhs->acc->number<uint64_t>() & 0xFF;
        return alu();
    }

    alu xio::ku16()
    {
        *acc = rhs->acc->number<uint64_t>() & 0xFFFF;
        return *acc;
    }

    alu xio::ku32()
    {
        *acc = rhs->acc->number<uint64_t>() & 0xFFFFFFFF;
        return *acc;
    }

    alu xio::ku64()
    {
        *acc = (uint64_t)(rhs->acc->number<uint64_t>());// & 0xFFFFFFFFFFFFFFFF);
        return *acc;
    }

    alu xio::ki8()
    {
        *acc = (int8_t)(rhs->acc->number<int64_t>() & 0xFF);
        return *acc;
    }

    alu xio::ki16()
    {
        logger::debug() << rhs->t0->text();
        *acc = (int16_t)(rhs->acc->number<uint64_t>() & 0xFFFF);
        return *acc;
    }

    alu xio::ki32()
    {
        *acc = (int32_t)(rhs->acc->number<uint64_t>() & 0xFFFFFFFF);
        return *acc;
    }

    alu xio::ki64()
    {
        *acc = (int64_t)(rhs->acc->number<int64_t>() & 0xFFFFFFFFFFFFFFFF);
        return *acc;
    }

    alu xio::kreal()
    {
        *acc = rhs->acc->number<double>();
        return *acc;
    }

    alu xio::kstring()
    {
        *acc = std::string(*rhs->acc);
        return *acc;
    }


    std::string xio::type_name()
    {
        return vxio::type::name(t0->t);
    }

    xio* xio::tree_rpar_input_postfix(xio*) {
        return {};
    }



    void xio::trace_tree_start(iostr& a_out, const iostr& Title)
    {
        a_out << "digraph vxio_club_expr_tree {\n";
        a_out << "ratio=compress; ranksep=.55; size = \"6.5,6.5\";\n";
        a_out << "    node [fontname=\"Source Code Pro\", fontsize=12];\n";
        a_out << "    label=\"Expression AST:[" << Title << "]\"; fontsize = 10;\n";
    }

    void xio::trace_node(xio* A, iostr& a_out)
    {
        //static int nullcount = 0;
        if (!A) return;
        if ((!A->lhs) && (!A->rhs)) return;

        if (A->lhs) {
            a_out << "    A" << A << " -> A" << A->lhs << ";\n";
            xio::trace_node(A->lhs, a_out);
        }
        //     else
        //         t_node::trace_null_node(A, nullcount++, a_stream);

        if (A->rhs) {
            a_out << "    A" << A << " -> A" << A->rhs << ";\n";
            xio::trace_node(A->rhs, a_out);
        }
        //     else
        //         trace_null_node(A, nullcount++, a_stream);
    }

    void xio::trace_null_node(xio*, int, iostr&)
    {

    }

    void xio::trace_tree(xio* a_root, iostr& a_out)
    {
        a_root->tree_attr(a_out);
        xio::trace_node(a_root, a_out);
    }

    void xio::trace_tree_close(iostr& a_out)
    {
        a_out << "}\n";
    }

    void xio::tree_attr(iostr& a_out)
    {
        iostr attr;
        attr << t0->text();
        if (t0->t & vxio::type::assign_t)
            attr << ": [" << (*acc)() << "]";
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
            lhs->tree_attr(a_out);
        if (rhs)
            rhs->tree_attr(a_out);
    }

    std::string xio::attribute() {
        return t0 ? t0->text() : "''";
    }


/// <summary>
/// 
///          +      
///         / \
///        va vari
/// </summary>
/// <returns></returns>
 std::string xio::trace_connect_binary_operands(xio* x)
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

    /// <summary>
    /// 
    /// +------*----+
    /// |     / \   |
    /// |    /   va |
    /// |variable   |
    /// +-----------+            
    ///        *   
    ///       / \  
    ///      /   va 
    ///  variable  
    ///                        

    ///              
    /// </summary>
    /// <param name="x"> pointer to the xio</param>
    /// <returns>2D string buffer</returns>
    // std::string xio::trace_connect_binary_operands2(xio* x)
    // {
    //     int lhs_w=1, rhs_w=1, lhs_mid=1, rhs_mid=1;

    //     if(x->lhs) 
    //         lhs_w = x->lhs->t0->text().length(); // 8
    //     if(x->rhs)
    //         rhs_w = x->rhs->t0->text().length(); // 8
        
    //     if(x->lhs)
    //         lhs_mid = lhs_w / 2 + (((lhs_w % 2) == 0) ? 1 : 0); // left index position
    //     if(x->rhs)
    //         rhs_mid = rhs_w / 2 - (((rhs_w % 2) == 0) ? 1 : 0); // right index position

    //     return "not implement";
    // }


    xio* xio::query_child(xio* cThis)
    {
        for (auto *i : _children) if (cThis == i) return i;
        return nullptr;
    }

}
