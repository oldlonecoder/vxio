//
// Created by oldlonecoder on 21-12-07.
//


#include <vxio/util/logger.h>


int main()
{
    rem::init();
    
    iostr str = "Hello, this is the %svxio %s(%sV%sirtual e%sX%secutable %sI%snstruction %sO%sbject%s) %sproject%s!\n";
    str << vxio::color::Lime << vxio::color::White << vxio::color::Yellow << vxio::color::Aqua << vxio::color::Yellow <<
    vxio::color::Aqua << vxio::color::Yellow << vxio::color::Aqua << vxio::color::Yellow << vxio::color::Aqua << vxio::color::White <<
    vxio::color::Lime << vxio::color::White << vxio::color::Reset;
    logger::debug() << str;
    
    logger::clear([](rem& r){
       std::cout << r.cc();
    });
    return 0;
}