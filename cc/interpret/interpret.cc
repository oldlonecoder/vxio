//
// Created by oldlonecoder on 21-12-11.
//

#include <vxio/interpret/interpret.h>
#include <vxio/util/logger.h>
#include <vxio/interpret/compiler/grammar.h>


namespace vxio
{


grammar gram;

vxio::interpret::~interpret()
{

}


rem::code vxio::interpret::init()
{
    //...
    gram.build();
    gram.Dump();
    _main = new vxu(this);
    //...
    return rem::code::implement;
}



alu interpret::operator[](std::string src_)
{
    if(!_main)
    {
        logger::error() << "interpret was not initialised.";
        return 0.0f;
    }
    _main->_config = {
        "../../res/main.vxu", // dummy
        src_.c_str()
    };
    _main->compile();
    //...
    return 42; // return nothing... (0.lf)
}

}