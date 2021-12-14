//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/compiler/expression.h>
#include <vxio/util/logger.h>

namespace vxio
{


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
        
};

expression::node::result expression::node::begin_ast(token_data &token)
{
    return logger::warning(src_funcname) << rem::code::_fn_ << ": implement";
}

expression::node::result expression::node::close_ast(token_data &token)
{
    return logger::warning(src_funcname) << rem::code::_fn_ << ": implement";
}

expression::node::result expression::node::input(token_data &token)
{
    for(auto [lr,fnptr] : expression::node::lr_input_table)
    {
        auto [l,r] = lr;
        if((token.s & r) && (this->token->s & l))
        {
            if(fnptr)
            {
                node * new_node = new node;
                new_node->token = &token;
                return (this->*fnptr)(new_node);
            }
        }
    }
    return logger::warning(src_funcname) << rem::code::_fn_ << ": implement";
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
    
    std::cout << __PRETTY_FUNCTION__ << "// maintenant on peut parcourir les tokens et construire l'arbre binaire de l'expression:";
    // maintenant on peut parcourir les tokens et construire l'arbre binaire de l'expression:
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
    if(token->d < n->token->d)
    {
        if(op)
            return op->op_input_binary_op(n);
        return set_right(n);
    }
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
    
    return logger::syntax(src_funcname) << rem::code::_fn_ << ": unexpected open par on input: unimplemented yet: \n" << n->token->mark();
}

expression::node::result expression::node::input_leaf_open_pair(expression::node *n)
{
    return logger::syntax(src_funcname) << rem::code::_fn_ << ": unexpected open par on input: unimplemented yet: \n" << n->token->mark();
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
    return logger::warning(src_funcname) << rem::code::_fn_ << ": implement";
}


expression::node::result expression::node::input_fncall_open_pair(expression::node *n)
{
    return logger::warning(src_funcname) << rem::code::_fn_ << ": implement";
}




expression::node::result expression::node::set_left(expression::node *n)
{
    n->op = this;
    ls = n;
    return this;
}


//  a * r^2
//     *  <- ^      *
//    / \          / \
//   g   h        g   ^  <- 2
//                   / \
//                  h   2

expression::node::result expression::node::set_right(expression::node *n)
{
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
    return set_right(n);
}

}