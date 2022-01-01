//
// Created by oldlonecoder on 21-11-20.
//

//#ifndef VXIO_FRAMEWORK_REM_H
//#define VXIO_FRAMEWORK_REM_H
#pragma once
#include <vxio/util/iostr.h>
#include <vxio/util/source_location.h>

class VXIO_API_DLL rem
{
    iostr _text;
    source::location _src = {};
    vxio::color::format _format = vxio::color::format::ansi256;
    std::vector<std::string> _components;
    
public:
    
    enum class type : int8_t
    {
        err,
        warning,
        info,
        exception,
        fatal,
        status,
        debug,
        output,
        comment,
        syntax
    };
    
    enum class code : uint16_t
    {
        hello,
        ok,
        rejected,
        accepted,
        eof,
        eos,
        null_ptr,
        not_empty,
        empty,
        unexpected,
        expected,
        endl,
        implement,
        _file_,
        _function_,
        _fn_,
        _line_
    };
    
    
    using collection = std::vector<rem>;
    
    rem()=default;
    rem(const rem& r);
    rem(rem&& r) noexcept;
    
    rem(rem::type type_, const source::location& src={});
    ~rem();
    
    
    rem& operator = (const rem&) = default;
    rem& operator = (rem&&) noexcept = default;
    
    rem& operator << (rem::code c_);
    rem& operator << (const iostr& txt_);
    template<typename T> rem& operator<<(const T& arg_)
    {
        
         iostr str;
         str << arg_;
         _components.push_back(str());
        return *this;
    }
    std::string cc();
    static void init();
    rem::code rem_code() { return _code; }
    rem::type rem_type() { return _type; }
    
private:
    rem::code _code = rem::code::hello;
    rem::type _type = rem::type::output;
    
    static std::vector<std::string_view> types_text;
    static std::vector<std::string_view> codes_text;
    static std::vector<std::string>      types_ansi256_attr;
    static std::vector<std::string>      codes_ansi256_attr;
    friend class logger;
    static void init_text_attr();
    
};
