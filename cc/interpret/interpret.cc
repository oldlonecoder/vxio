//
// Created by oldlonecoder on 21-12-11.
//

#include <vxio/interpret/interpret.h>
#include <vxio/util/logger.h>
//#include <vxio/interpret/compiler/grammar.h>


namespace vxio
{

static interpret* interpreter = nullptr;
static grammar gram;

vxio::interpret::~interpret()
{

}


rem::code vxio::interpret::init()
{
    //...
    interpreter = this;
    gram.build();
    gram.dump();
    _main = new vxu(this);
    //...
    return rem::code::implement;
}



alu interpret::operator[](std::string src_)
{
    if(!_main)
    {
        logger::error() << "vxio interpret was not initialised.";
        return 0.0f;
    }
    _main->_config = {
        "../../res/main.vx", // dummy
        src_.c_str() // for real
    };
    _main->compile();
    
    //...
    return 42; // return nothing... (0.lf)
}



const grammar::rule *interpret::get_rule(const std::string &name_)
{
    return gram[name_];
}


interpret *interpret::instance()
{
    return interpreter;
}

}
