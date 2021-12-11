//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/compiler/expression.h>
#include <vxio/util/logger.h>



namespace vxio
{

expression::~expression()
{

}

rem::code expression::parse(context ctx_)
{
    _ctx = std::move(ctx_);
    //Rassembler les tokens de l'expression arithm&eacute;tique:
    while((_ctx.finish <= _ctx.last) && _ctx.finish->_flags.V) _ctx.i_tokens.push_back(_ctx.finish++);
    if(_ctx.i_tokens.empty())
    {
        logger::error() << ": expression parser: no tokens";
        return rem::code::empty;
    }
    
    // maintenant on peut parcourir les tokens et construire l'arbre binaire de l'expression:
    xio* input = nullptr;
    for(auto i_token : _ctx.i_tokens)
    {
        if(!input)
        {
            input = xio::begin_expr(&(*i_token), [this](token_data* ptok)->xio*{
                return create_xio(ptok);
            });
            continue;
        }
        input = input->tree_input(&(*i_token), [this](token_data* ptok)->xio*{
                return create_xio(ptok);
            });
    }
    return rem::code::accepted;
}

xio *expression::create_xio(token_data *token_)
{
    switch(token_->t)
    {
        //...
        default:
            return new xio(_ctx.blk, token_, nullptr);
    }
    return nullptr;
}

}