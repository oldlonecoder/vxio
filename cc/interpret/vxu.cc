//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/vxu.h>
#include <vxio/util/logger.h>


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
}
