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
    std::cout << "debug (check _ctx): " << _ctx.describe() << "\n";
    
    //Rassembler les tokens de l'expression arithm&eacute;tique:
    
    std::cout << __PRETTY_FUNCTION__ << ": //Rassembler les tokens de l'expression arithm&eacute;tique:\n";
    auto i = _ctx.start;
    while((i <= _ctx.last) && i->_flags.V) _ctx.i_tokens.push_back(i++);
    
    if(_ctx.i_tokens.empty())
    {
        logger::error() << ": expression parser: no tokens";
        return rem::code::empty;
    }
    
    std::cout << __PRETTY_FUNCTION__ << "// maintenant on peut parcourir les tokens et construire l'arbre binaire de l'expression:";
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
    _ctx.finish = _ctx.last;
    
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