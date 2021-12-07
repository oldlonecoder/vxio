//
// Created by oldlonecoder on 21-11-20.
//

#include <vxio/util/rem.h>


std::vector<std::string_view> rem::types_text=
{
    {"error"},
    {"warning"},
    {"information"},
    {"exception"},
    {"fatal"},
    {"status"},
    {"debug"  },
    {"output"},
    {"comment"},
    {"syntax error"}
};

std::vector<std::string_view> rem::codes_text=
{
    {"hello"} ,
    {"ok"} ,
    {"rejected"} ,
    {"accepted"} ,
    {"end of file"} ,
    {"end of stream"} ,
    {"null pointer"} ,
    {"not empty"} ,
    {"empty"} ,
    {"unexpected"},
    {"expected"},
    {"\n"},
    {"implement"}

};


std::vector<std::string>      rem::types_ansi256_attr;
std::vector<std::string>      rem::codes_ansi256_attr;

rem::rem(const rem &r)
{
    _text = r._text;
    
}
rem::rem(rem &&r) noexcept
{
    _text = std::move(r._text);
    
}
rem::~rem()
{
    _text.clear();
}
rem::rem(rem::type type_, const source::location &src)
{
    _type = type_;
    _src = src;
}

rem &rem::operator<<(rem::code c_)
{
    if(c_ < rem::code::_file_ )
        _components.emplace_back(rem::codes_text[static_cast<int8_t>(_type)].data());
    // bypass code value from _file_.
    return *this;
}


void rem::init_text_attr()
{
    if(!rem::types_ansi256_attr.empty()) return;
    rem::types_ansi256_attr =
    {
        vxio::color::ansi(vxio::color::DarkRed2),       // err,
        vxio::color::ansi(vxio::color::Yellow),         //warning,
        vxio::color::ansi(vxio::color::BlueViolet),     //info,
        vxio::color::ansi(vxio::color::Cyan3),          //exception,
        vxio::color::ansi(vxio::color::DarkBlue),       //fatal,
        vxio::color::ansi(vxio::color::DarkSlateGray1), //status,
        vxio::color::ansi(vxio::color::LightCoral),     //debug,
        vxio::color::ansi(vxio::color::White),          //output,
        vxio::color::ansi(vxio::color::Grey0),          //comment,
        vxio::color::ansi(vxio::color::DeepPink1),      //syntax
    };
    rem::codes_ansi256_attr =
    {
        vxio::color::ansi(vxio::color::Lime),           //hello,
        vxio::color::ansi(vxio::color::LightCyan3),     //ok,
        vxio::color::ansi(vxio::color::Red3),           //rejected,
        vxio::color::ansi(vxio::color::LightSeaGreen),  //accepted,
        vxio::color::ansi(vxio::color::LightPink4),     //eof,
        vxio::color::ansi(vxio::color::LightPink3),     //eos,
        vxio::color::ansi(vxio::color::Grey0),          //null_ptr,
        vxio::color::ansi(vxio::color::LightGoldenrod2),//not_empty,
        vxio::color::ansi(vxio::color::Grey)            ,//empty,
        vxio::color::ansi(vxio::color::LightSteelBlue3),//unexpected,
        vxio::color::ansi(vxio::color::LightSteelBlue3),//expected,
        vxio::color::ansi(vxio::color::White),          //_file_,
        vxio::color::ansi(vxio::color::LightSteelBlue),//_function_,
        vxio::color::ansi(vxio::color::LightSteelBlue3),//_fn_,
        vxio::color::ansi(vxio::color::White),          //_line_
    };
}
std::string rem::cc()
{
    iostr str;
    str << rem::types_ansi256_attr[static_cast<int8_t>(_type)];
    str << rem::types_text[static_cast<int8_t>(_type)];
    str << vxio::color::ansi(vxio::color::White);
    str << ": ";
    
    if(!_src.filename.empty())
    {
        str << "\n";
        str << rem::codes_ansi256_attr[static_cast<int16_t>(rem::code::_file_)];
        iostr::word::list_t words;
#ifndef _WIN32
        if(str.words(words,"/") > 0)
#else
        if (str.words(words, "\\") > 0)
#endif
            str << words.back()();
        else
            str << str();
        str << ' ';
    }
    
    if(!_src.func_name.empty())
    {
        str << rem::codes_ansi256_attr[static_cast<int16_t>(rem::code::_function_)];
        str << ": ";
    }
    
    if(_src.line_no > 0)
    {
        str << rem::codes_ansi256_attr[static_cast<int16_t>(rem::code::_line_)];
        str << _src.line_no;
        str << ": ";
    }
    
    for(auto const& s : rem::_components)
    {
        str << s;
        str << ' ';
    }
    return str();
}
rem &rem::operator<<(const iostr &txt_)
{
    _components.push_back(txt_());
    return *this;
}
void rem::init()
{// ...
    rem::init_text_attr();
}