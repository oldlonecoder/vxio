//
// Created by oldlonecoder on 21-12-07.
//


#include <vxio/util/logger.h>
#include <vxio/lexer/lexer.h>
#include <vxio/interpret/interpret.h>

namespace vxio
{

rem::code test_interpret()
{
    interpret i;
    logger::debug() << " init the interpret:";
    
    i.init();
    logger::debug() << " invoke the interpret:";
    logger::debug() << "alu a = i[\"0.03 * 4ac(8 + 5/6) -1;\"];";
    alu a = i["0.03 * 4ac(8 + 5/6) -1;"];
    logger::debug() << " interpret is done (in case there is no debug infos... \nLeaving [" << a() << "].";
    return rem::code::ok;
}

rem::code test_lexer()
{
    lexer lex;
    token_data::collection tokens;
    lex.config() =
    {
        "static u64 a = 0.03 * 4(8 + 5/6) -1;",
        &tokens
    };
    auto r = lex();
    if(r != rem::code::accepted)
    {
        logger::error() << " lexer rejected token somewhere...";
    }
    lex.dump_tokens([](const token_data& token) {
       std::cout <<  token.details() << ":\n" << token.mark() << '\n';
    });
    
    return rem::code::ok;
}

}
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
    
    
    (void)vxio::test_lexer();
    (void)vxio::test_interpret();
    
    logger::clear([](rem& r){
       std::cout << r.cc() << '\n';
    });
    return 0;
}