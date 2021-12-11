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

vxu::vxu(std::string fname_):bloc(nullptr,nullptr,nullptr),
_fname(std::move(fname_)){}

alu vxu::jsr()
{
    return bloc::jsr();
}
rem::code vxu::load_code()
{
    
    
    return rem::code::implement;
}
}
