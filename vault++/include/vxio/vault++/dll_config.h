#pragma once


#ifdef _WIN32
#   pragma warning(disable : 4996)
#   pragma warning(disable : 4005)
#   pragma warning(disable : 4267) 
//class __declspec(dllimport) std::thread;
#   define _CRT_SECURE_NO_WARNINGS
#   define _SCL_SECURE_NO_WARNINGS

#   ifdef VXIO_VAULT_DLL_EXPORT
#       define VXIO_VAULT_DLL __declspec(dllexport)
#       if __GNUC__ >= 4
#           define VXIO_VAULT_DLL __attribute__ ((visibility ("default")))
#           define PRIVATE_LIB  __attribute__ ((visibility ("hidden")))
//#       else
//#           define VXIO_VAULT_DLL
//#           define PRIVATE_LIB
#       endif
#   else
#       define VXIO_VAULT_DLL __declspec(dllimport)
#   endif
#   else
#       define VXIO_VAULT_DLL    // Not ms-Windows/MSVC
#endif

// Mandatory. Globally used where it needs to.

//---------------------------------------------


// --  Basic pseudo reflexion macros : --
#ifndef DECLOBJ_ID
#define DECLOBJ_ID \
    public:\
        virtual const char*  class_name() const noexcept; \
    private:\


#define  CLASSNAME_IMPL(CLASS) \
         const char*   CLASS::class_name() const noexcept{ return #CLASS; }
#endif
//----------------------------------------------------------------------------


#ifndef _WIN32
#   define _strdup  strdup

#   include <thread>
#   include <cstdint>

#else
#   define __PRETTY_FUNCTION__ __FUNCSIG__
#endif
//#endif //TKERNEL_DLL_H

