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
    enum class type : int8_t
    {
        err,
        warning,
        info,
        exception,
        fatal,
        status,
        debug,
        output
    };
    
    
    source::location src = {};

    
    using stack = std::vector<message>;
    std::vector<std::string> components;
    
    message(){ format = color::format::ansi256; } // Forcer pour verifier un crash.
    message(diagnostic::type_t a_type, diagnostic::code_t a_code);
    //message(diagnostic::type_t a_type) { type = a_type; }
    message(message&&) = default;
   
    message(diagnostic::type_t a_type, const source::location& a_location = {});
    explicit message(diagnostic::code_t a_code);
    message(const message&) = default;
    
    virtual ~message();
    
    message& operator = (message&&) = default;
    message& operator = (const message&) = default;
    
    message& operator << (diagnostic::code_t a_code);
    message& operator << (diagnostic::type_t a_type);
    template<typename T> diagnostic::message& operator << (const T& a_val)
    {
        cstr stext;
        stext << a_val;
        components.push_back(stext());
        return *this;
    }
    static message nil;
    
    virtual std::string cc(color::format frmt = color::format::ansi256);
    std::string render_type(diagnostic::type_t t_);
    std::string render_code(diagnostic::code_t t_);
};



//#endif //VXIO_FRAMEWORK_REM_H
