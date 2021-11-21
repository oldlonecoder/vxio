//
// Created by oldlonecoder on 21-11-20.
//

//#ifndef VXIO_FRAMEWORK_REM_H
//#define VXIO_FRAMEWORK_REM_H
#pragma once
#include <vxio/util/iostr.h>
#include <vxio/util/source_location.h>

class VXIO_UTIL_DLL rem
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
    
    
    using list = std::vector<rem>;
    
    rem()=default;
    rem(const rem& r);
    rem(rem&& r) noexcept;
    
    rem(rem::type type_, const source::location& src={});
    ~rem();
    
    
    rem& operator = (const rem&) = default;
    rem& operator = (rem&&) noexcept = default;
    
    rem& operator << (rem::code c_);
    
    template<typename T> rem& operator<<(const T& arg_)
    {
        
         iostr str;
         str << arg_;
         _components.push_back(str());
        return *this;
    }
    std::string cc();
private:
    rem::code _code = rem::code::hello;
    rem::type _type = rem::type::output;
    
    static std::vector<std::string_view> types_text;
    static std::vector<std::string_view> codes_text;
    static std::vector<std::string>      types_ansi256_attr;
    static std::vector<std::string>      codes_ansi256_attr;
    friend class logger;
    static void init_text_attr();
    
    
//    using stack = std::vector<message>;
//    std::vector<std::string> components;
//
//    message(){ format = color::format::ansi256; } // Forcer pour verifier un crash.
//    message(diagnostic::type_t a_type, diagnostic::code_t a_code);
//    //message(diagnostic::type_t a_type) { type = a_type; }
//    message(message&&) = default;
//
//    message(diagnostic::type_t a_type, const source::location& a_location = {});
//    explicit message(diagnostic::code_t a_code);
//    message(const message&) = default;
//
//    virtual ~message();
//
//    message& operator = (message&&) = default;
//    message& operator = (const message&) = default;
//
//    message& operator << (diagnostic::code_t a_code);
//    message& operator << (diagnostic::type_t a_type);
//    template<typename T> diagnostic::message& operator << (const T& a_val)
//    {
//        cstr stext;
//        stext << a_val;
//        components.push_back(stext());
//        return *this;
//    }
//    static message nil;
//
//    virtual std::string cc(color::format frmt = color::format::ansi256);
//    std::string render_type(diagnostic::type_t t_);
//    std::string render_code(diagnostic::code_t t_);
};



//#endif //VXIO_FRAMEWORK_REM_H
