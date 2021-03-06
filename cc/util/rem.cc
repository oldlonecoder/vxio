//
// Created by oldlonecoder on 21-11-20.
//

#include <vxio/util/rem.h>


int rem::_indent = 0;

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
    {"begin"},
    {"end"},
    {"implement"}

};


std::vector<std::string>      rem::types_ansi256_attr;
std::vector<std::string>      rem::codes_ansi256_attr;

rem::rem(const rem &r)
{
    _text = r._text;
    _type = r._type;
    _code = r._code;
    _src = r._src;
    _components = r._components;
    
}

rem::rem(rem &&r) noexcept
{
    _src = std::move(r._src);
    _text = std::move(r._text);
    _components = std::move(r._components);
    _type = r._type;
    _code = r._code;
    
}

rem::~rem()
{
    _text.clear();
    _components.clear();
}


/*!
@brief Constructs an instance of "rem" which consists of the following stacked elements, formatted according to the encoding:
 <ul>
    <li> message type </li>
    <li> mnemonic code </li>
    <li> message text </li>
 </ul>
@param type_  rem::type mnemonic
@param src    optional (non-empty) code source location given by \c source::location
 */
rem::rem(rem::type type_, const source::location &src)
{
    _type = type_;
    _src = src;
    if(!src.func_name.empty())
    {
        // Use "src_funcname" coorectly soon...;
        ++rem::_indent;
        //...
    }
}

/*!
 * @brief input rem::code
 * @param c_ the rem::code mnemonic
 * @return reference to instance of this for chaining inputs.
 *
 * @note Production:
 <ul>
     <li> html format: </li>
     <ul>
         <li> openning span or div html element with the css class: "rem-code"</li>
         <li> text of the code mnemonic </li>
         <li> span/div closing tag </li>
     </ul>
     <li> ansi256 format:
     <ul>
        <li> ansi text attr</li>
        <li> code string text </li>
     </ul>
  </ul>
 
 */
rem &rem::operator<<(rem::code c_)
{
    iostr str;
    int i = static_cast<int>(c_);
    str << rem::codes_ansi256_attr[i] << codes_text[i].data();
    _components.emplace_back(str());
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
        vxio::color::ansi(vxio::color::DeepSkyBlue7),   //fatal,
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
        vxio::color::ansi(vxio::color::Reset),          //endl,
        vxio::color::ansi(vxio::color::Reset),          //begin,
        vxio::color::ansi(vxio::color::Reset),          //end,
        vxio::color::ansi(vxio::color::White),          //implement,
        vxio::color::ansi(vxio::color::White),          //_file_,
        vxio::color::ansi(vxio::color::LightSteelBlue3),//_function_,
        vxio::color::ansi(vxio::color::LightSteelBlue3),//_fn_,
        vxio::color::ansi(vxio::color::White),          //_line_
    };
}

/*
 *
 *  <div class="nom de la classe"...> contenu </div>
 *  <span class="nom de la classe"...> contenu </span>
 *  <i class="icone id-icon">text content</i>
 *  <script> code javascript </script>
 */

std::string rem::cc()
{
    iostr str;
    str += rem::types_ansi256_attr[static_cast<int8_t>(_type)];
    str += rem::types_text[static_cast<int8_t>(_type)];
    str += vxio::color::ansi(vxio::color::White);
    str += ' ';
    
    if(!_src.filename.empty())
    {
        //str += '\n';
        str += rem::codes_ansi256_attr[static_cast<int16_t>(rem::code::_file_)];
        iostr fname = _src.filename;
        iostr::word::list_t words;
#ifndef _WIN32
        if(fname.words(words,"/") > 0)
#else
        if (fname.words(words, "\\") > 0)
#endif
            str += words.back()();
        else
            str += fname();
        str += ' ';
    }
    if(_src.line_no > 0)
    {
        str << ": " << vxio::color::White << _src.line_no << ' ' << rem::codes_ansi256_attr[static_cast<int16_t>(rem::code::_line_)] << ' ';
    }

    if(!_src.func_name.empty())
    {
        str += rem::codes_ansi256_attr[static_cast<int16_t>(rem::code::_function_)];
        str+= _src.func_name;
        str += ": ";
    }
    
    for(auto const& s : rem::_components)
    {
        str += s;
        //str << ' ';
    }
    _text = str;
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
std::string rem::code_text(rem::code c)
{
    std::string str;
    if(c == rem::code::_enter_)
    {
        ++rem::_indent;
        return "";
        //...
    }
    else
        if(c==rem::code::_leave_)
        {
            --rem::_indent;
            return "";
            //...
        }
        
    str += rem::codes_ansi256_attr[static_cast<int8_t>(c)];
    str += rem::codes_text[static_cast<int8_t>(c)];
    return str;
}

std::string rem::type_text(rem::type t)
{
    std::string str;
    str += rem::types_ansi256_attr[static_cast<int8_t>(t)];
    str += rem::types_text[static_cast<int8_t>(t)];
    return str;
}

