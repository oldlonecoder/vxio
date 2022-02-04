//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/vxu.h>
#include <vxio/util/logger.h>



namespace vxio
{

using compiler::context_data;


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
    cc_rule(nullptr);

    return rem::code::implement;
//    return r;
}

rem::code vxio::vxu::cc_rule(const grammar::rule* _rule)
{
    throw logger::fatal(src_location) << rem::code::implement << " grammar::rule '" << _rule->_id ;
}

rem::code vxio::vxu::cc_sequence(const grammar::term_seq& sequence)
{
    throw logger::fatal(src_location) << rem::code::implement << " cannot be called outise of vxu::cc_rule!";
}


}


