//
// Created by oldlonecoder on 21-12-11.
//

#include <vxio/interpret/interpret.h>
#include <vxio/util/logger.h>


namespace vxio
{

vxio::interpret::~interpret()
{

}

rem::code vxio::interpret::init()
{
    _main = new vxu(this);
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
        "../../res/test.vxu",
        "a = 0.03 * 4ac(8 + 5/6) -1;"
    };
    _main->compile();
    
}

}