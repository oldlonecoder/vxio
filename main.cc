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
    str = "╭";
    
    
    logger::info() << " Some data informations:\n" <<
        vxio::color::White << "Size of std::string :" << vxio::color::Yellow << sizeof(std::string) << "\n...";
    logger::info() << vxio::color::White << "length of '╭':" << vxio::color::Yellow << str.length() << '\n';
    
    void* p = (void*)str.c_str();
    iostr hx = "| %08b |";
    uint32_t* t = (uint32_t*)p;
    hx << *t;
    logger::info() << vxio::color::White << "uint64_t: " << vxio::color::Yellow << hx() << '\n';
    
    logger::clear([](rem& r){
       std::cout << r.cc();
    });
    return 0;
}