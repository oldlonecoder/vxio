//
// Created by oldlonecoder on 21-12-07.
//


#include <vxio/util/logger.h>


int main()
{
    rem::init();
    
    iostr str = "Hello, this is the %svxio %s(%sV%sirtual e%sX%secutable %sI%snstrucyion %sO%sbject%s) %sworld!\n";
    str << vxio::color::Lime << vxio::color::Aqua << vxio::color::White << vxio::color::Fuchsia << vxio::color::Yellow <<
    vxio::color::Fuchsia << vxio::color::Yellow << vxio::color::Fuchsia << vxio::color::Yellow << vxio::color::Fuchsia << vxio::color::Yellow <<
    vxio::color::Fuchsia << vxio::color::Yellow <<vxio::color::Fuchsia << vxio::color::Yellow;
    logger::debug() << str;
    
    logger::clear([](rem& r){
       std::cout << r.cc();
    });
    return 0;
}