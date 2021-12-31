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
    //... fake
    _id = _config.filename;
    // duh!
    std::cout << __PRETTY_FUNCTION__ << " ID: '" << _id << "'\n";
    return rem::code::implement;
}


rem::code vxu::compile()
{
    return rem::code::implement;
//    logger::debug() << __PRETTY_FUNCTION__ << "[" << color::Yellow << _id << color::Reset << "]:";
//    lexer lex;
//    lex.config() = {
//        _config.source,
//        &_tokens
//    };
//    load_code(); ///... fake, but sets _id to _fname.
//    logger::debug() << __PRETTY_FUNCTION__ << "[" << color::Yellow << _id << color::Reset << "]:";
//    auto r = lex();
//    logger::debug() << ": lexer done " << _tokens.size() << " tokens.";
//    if(r!=rem::code::accepted)
//    {
//        logger::error() << " the lexer rejected the source";
//        return r;
//    }
//    expression cc;
//    logger::debug() << ": now parsing expression:";
//    std::cout << __PRETTY_FUNCTION__ << " now parsing expression:\n";
//    r = cc.parse({this, _tokens.begin(),--_tokens.end()},nullptr);
//    // ...
//    return r;
}

}
