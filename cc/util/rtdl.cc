#include <vxio/util/rtdl.h>
#include <vxio/util/logger.h>
#include <unistd.h>

namespace vxio::dll
{


CLASSNAME_IMPL(rtdl)

rtdl::rtdl(object* parent_, const std::string& id_) :object(parent_),
_id(id_)
{

}

rtdl::~rtdl()
{
}

dll_file::dll_file(object* parent_, const std::string& id_) :object(parent_), _id(id_)
{
}

#ifdef _WIN32


std::string dll_file::locate()
{
    iostr  file = "%s.dll";
    file << _id;
    WIN32_FIND_DATA FindFileData;
#ifdef _UNICODE
    if (HANDLE handle = FindFirstFile((LPCWSTR)file().c_str(), &FindFileData); handle != INVALID_HANDLE_VALUE)
#else
#ifdef UNICODE
    if (HANDLE handle = FindFirstFile((LPCWSTR)file().c_str(), &FindFileData); handle != INVALID_HANDLE_VALUE)
#else
    if (HANDLE handle = FindFirstFile((LPCSTR)file().c_str(), &FindFileData); handle != INVALID_HANDLE_VALUE)
#endif
#endif
    FindClose(handle);
    return file();
}

rem::code dll_file::open()
{
    std::string str_location = locate();
    if (str_location.empty()) {
        //return { (
        //    Annotation::Push(),
        //    Annotation::Type::Error,' ',
        //    "Library file '", _ID, "' ",
        //    Annotation::Code::NotFound,
        //    " Within the system paths."
        //) };
        //throw  message::push(message::t::error), id, " not found into valid paths.";
        return rem::code::null_ptr;
    }
#if defined(_UNICODE) || defined(UNICODE)
    _Handle = LoadLibrary((LPCWSTR)str_location.c_str());
#else
    _Handle = LoadLibrary((LPCSTR)str_location.c_str());
#endif
    if (!_Handle) {
        // ---------------------------------------------------------------------------------------
        //https://stackoverflow.com/a/17387176 --
        // Modified.
        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        std::string SysMsg(messageBuffer, size);

        //Free the buffer.
        LocalFree(messageBuffer);
        // ---------------------------------------------------------------------------------------
        //return { (
        //    Annotation::Push(),
        //    Annotation::Type::Error,
        //    Annotation::Code::Null,
        //    "Openning ", _ID, "Library file: [",SysMsg, "]."
        //) };
        std::cerr << SysMsg << "\n";
        return rem::code::empty;
    }
    //throw  message::push(message::t::error), id, ": dll error";

//GetProcAddress;
    FARPROC _export_fn = nullptr;
    _export_fn = GetProcAddress(_Handle, EXPORT_SYM);
    if (!_export_fn) {
        FreeLibrary(_Handle);
    LeaveWithNoInterface:
        //return { (
        //    Annotation::Push(),
        //    Annotation::Type::Error,
        //    Annotation::Code::Empty,
        //    " - rtdl ", _ID, ": does not - or failed to - provides interface."
        //) };
        //throw message::push(message::t::error), " rtdl ", id, ": rtdl does not provides exported interface.";
        return rem::code::null_ptr;
    }

    _interface = reinterpret_cast<rtdl::interface_map(*)()>(_export_fn)();
    if (_interface.empty()) {
        FreeLibrary(_Handle);
        goto LeaveWithNoInterface;
    }

    for (auto& ix : _interface) {
        ix.second = GetProcAddress(_Handle, ix.first.c_str());
        if (!ix.second) {
            FreeLibrary(_Handle);
            //return { (
            //    Annotation::Push(),
            //    Annotation::Type::Error,
            //    Annotation::Code::Null,
            //    "- rtdl '", _ID, " has no `exported` symbol identified by '", ix.first,"', while listed in its exported interface."
            //) };
            //throw message::push(message::t::error), " rtdl ", id, ": exported symbol is unbound: [", ix.first, "]";
            return rem::code::empty;
        }
    }
    if ((_rtdl = (reinterpret_cast<rtdl * (*)()>(_interface[CREATE_SYM]))()) != nullptr) {
        _rtdl->set_interface(_interface);
        _interface.clear(); // Do not keep a local copy since the interface data is now owned by the rtdl Instance...
    }
    return rem::code::ok;
}

int dll_file::close()
{
    if (!_Handle) return 0;
    if (_rtdl) release();
    FreeLibrary(_Handle);
    _Handle = nullptr;
    return 0;
}

int dll_file::release()
{
    if (!_rtdl)
        return 0;

    (void) reinterpret_cast<int(*)(rtdl*)>(_rtdl->_interface[DELETE_SYM])(_rtdl);
    _rtdl = nullptr;
    return 0;
}

#else

/*!
@brief There are only two places where we can find the library :
@li Under LD_LIBRARY_PATH paths lists
@li Under the current "./" working directory.
@return the absolute path and final filename in std::string.
@author Serge Lussier(oldlonecoder), lussier.serge@gmail.com
*/
std::string dll_file::locate()
{
    iostr::word::list_t a_paths;
    std::string str_format = "%s/lib%s.so";
    std::string str_file;
    iostr file = str_format;
    const char* cpath = getenv("LD_LIBRARY_PATH");
    iostr paths;
    if (!cpath)
        paths = "./";
    else
        paths << cpath << ":.";

    paths.words(a_paths, ":");

    for (iostr::word p : a_paths) {
        file = str_format;
        std::string w = p();
        std::string::size_type pos = w.find(":");
        if (pos != std::string::npos)
            w.erase(pos, 1);
        std::cout << " checking path: [" << w << "]:\n";
        file.arg(w).arg(_id);
        str_file = file();
        std::cout << " checking path: [" << str_file << "]:\n";
        if (!access(str_file.c_str(), F_OK)) {
            std::cout << " Location found: [" << str_file << "]:\n";
            return str_file;
        }
    }
    return "";
}


/*!
*    @brief Opens the library at the found location into the filesystem.
*
*    @return pointer to the Plugin base class object successfuly initialized and ready to use.
*    @author Serge Lussier(bretzelus), lussier.serge@gmail.com
*/
rem::code dll_file::open()
{

    std::string str_location = locate();
    if (str_location.empty()) {
        throw logger::fatal(src_location) <<  _id <<  " not found into valid paths.";
    }
    _Handle = dlopen(str_location.c_str(), RTLD_LAZY); // Recommended
    if (!_Handle)
        throw logger::fatal(src_location) <<  _id << ": " <<  dlerror();

    void* _export_fn = dlsym(_Handle, EXPORT_SYM);
    if (!_export_fn) {
        dlclose(_Handle);
        throw logger::fatal(src_location) <<  " Plugin " <<  _id <<  ": " <<  dlerror();
    }

    _interface = reinterpret_cast<rtdl::interface_map(*)()>(_export_fn)();
    if (_interface.empty()) {
        dlclose(_Handle);
        throw logger::fatal(src_location) << " Plugin " << _id <<  " does not export its interface! ";
    }

    for (auto& ix : _interface) {
        ix.second = dlsym(_Handle, ix.first.c_str());
        if (!ix.second) {
            dlclose(_Handle);
            throw logger::fatal(src_location) <<  _id << ": exported symbol is unbound: [" <<  ix.first <<  "]";
        }
    }

    _rtdl = (reinterpret_cast<rtdl * (*)()>(_interface[CREATE_SYM]))();
    _rtdl->set_interface(_interface);
    _interface.clear();
    return rem::code::accepted;
}


/*!
*    @brief Closes the library, and destroys its Plugin instance.
*
*    @author Serge Lussier(bretzelus), lussier.serge@gmail.com
*/
int dll_file::close()
{
    if (!_Handle) return 0;
    if (_rtdl);
    _Handle = 0;
    return 0;
}


int dll_file::release()
{
    if (!_rtdl)
        return 0;

    (void) reinterpret_cast<int(*)(rtdl*)>(_rtdl->_interface[DELETE_SYM])(_rtdl);
    _rtdl = nullptr;
    return 0;
}

#endif


}


/*

#pragma once

#include <Sll/SllPlugin.h>



class LexicalScannerPlugin : public Sll::Plugin::Plugin
{
public:
    LexicalScannerPlugin() = delete;
    ~LexicalScannerPlugin() override;

    Sll::Annotation::Code Run() override;
};


#include <LexicalScanner/LexerPlugin.h>




BEGIN_EXPORT


Sll::Expect<Sll::Annotation::Code> Scan(LexicalScannerPlugin& LSP)
{
    return { LSP.Run() };
}

Sll::Plugin::Plugin::InterfaceMap Export()
{
    Sll::Plugin::Plugin::InterfaceMap Interface = {
        {"Scan", Scan}
    };
    return Interface;
}

CREATE_PLUGIN_INSTANCE
{
    return nullptr;
}

EXPORT_ENDBLOC

Sll::Annotation::Code LexicalScannerPlugin::Run()
{
    return Sll::Annotation::Code::Implement;
}

...


#include <xio/lexer/rtlx_lexer.h>
#include <xio/lexer/lexer.h>
#include <map>


namespace teacc::lexer
{

using utils::rtlx;

teacc::lexer::rtlx_lexer::~rtlx_lexer()
{

}
teacc::lexer::rtlx_lexer::rtlx_lexer(const std::string &dlname) : rtlx(dlname)
{

}


utils::notification::code teacc::lexer::rtlx_lexer::execute()
{

    return utils::notification::code::implement;
}


}



BEGIN_EXPORT


std::string highlight(teacc::lexer::rtlx_lexer::shared _instance, const std::string& _src)
{
    if(!_instance) return "";
    teacc::lexer::lexscanners lexer;
    lexer.source() = _src.c_str();
    lexer.Scan();
    // .. for ( auto& t : lexer.tokens() )
    //{
    //     highlight ...
    //}
    return "this source is highlighted.";

}

CREATE_INSTANCE_C_IMPL
{
    return std::make_shared<teacc::lexer::rtlx_lexer>(aId);
};

DELETE_INSTANCE_C_IMPL
{
    _dll_instance = nullptr;
};

teacc::utils::rtlx::interface_map export_symbols()
{
    return {
        {"highlight",(void*)highlight}
    };

}




END_EXPORT

*/





