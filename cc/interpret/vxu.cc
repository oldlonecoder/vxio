//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/vxu.h>
#include <vxio/util/logger.h>
#include <vxio/interpret/compiler/expression.h>


namespace vxio
{

vxu::~vxu()
{
    _tokens.clear();
}

vxu::vxu(xio* parent_):bloc(parent_,nullptr,nullptr)
{}

alu vxu::jsr()
{
    return bloc::jsr();
}


rem::code vxu::load_code()
{
    return rem::code::implement;
}


rem::code vxu::compile()
{
    lexer lex;
    lex.config() = {
        _config.source,
        &_tokens
    };
    auto r = lex();
    if(r==rem::code::accepted)
        return r;
    
    expression cc;
    r = cc.parse({this, _tokens.begin(),--_tokens.end()});
    // ...
    return r;
}

}
