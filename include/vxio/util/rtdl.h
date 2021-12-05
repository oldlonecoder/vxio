#pragma once

//#include <vxio/dll_config.h>
#include <vxio/util/object.h>
//#include <vxio/util/iostr.h>
#include <vxio/util/rem.h>

#ifdef _WIN32
#include <Windows.h>
#else
#define FARPROC     void*
#define HINSTANCE   void*

#include <dlfcn.h>
#endif

#include <map>


#define BEGIN_EXPORT      extern "C"{
#define EXPORT_ENDBLOC    }

#define CREATE_SYM "_create_symbol"
#define DELETE_SYM "_delete_symbol"
#define EXPORT_SYM "_export"
#define EXPORT   vxio::dll::rtsl::interface_map export()
#define CREATE_INSTANCE vxio::dll::rtdl* _create(vxio::object* parent_, const std::string &id_)
#define DELETE_INSTANCE void _del(vxio::dll::rtdl* rtdl_)


namespace vxio::dll
{


class VXIO_API_DLL rtdl : public object
{
    DECLOBJ_ID
    std::string _id;
    friend class dll_file;
public:
    using interface_map = std::map<std::string, FARPROC>;
protected:
    interface_map _interface;
public:
    rtdl() = delete;
    rtdl(const rtdl&) = delete;
    rtdl(object* parent_, const std::string& id_);
    ~rtdl() override;
    

    template<typename _Ret, typename ..._Args> _Ret call(const std::string& symbol_id_, _Args...Args_)
    {
        using F = _Ret(*)(rtdl*, _Args...Args_);
        auto f = _interface.find(symbol_id_);
        if (f == _interface.end())
            return _Ret();

        F fn = reinterpret_cast<F>(f->second);
        if (fn)
            return fn(this, Args_...);
    }

    void set_interface(const rtdl::interface_map& i_) { _interface = i_; }
    virtual rem::code Run() = 0;
};




class VXIO_API_DLL dll_file  final : public object
{
    HINSTANCE _Handle = nullptr;
    std::string _id;
    static rtdl::interface_map _null;
    rtdl* _rtdl = nullptr;
    rtdl::interface_map _interface;

public:

    dll_file() {}
    dll_file(object* parent_, const std::string& id_);
    ~dll_file() = default;
    std::string locate();
    rem::code open();
    int close();
    int release();
};


}


/*


#include <map>

#ifdef _WIN32
#include <Windows.h>
#else
#define FARPROC     void*
#define HINSTANCE   void*

#include <dlfcn.h>
#endif


namespace Sll::Plugin {

#define BEGIN_EXPORT      extern "C"{
#define EXPORT_ENDBLOC    }

#define CREATE_PLUGIN_SYM "__Create"
#define DELETE_PLUGIN_SYM "__Delete"
#define EXPORT_SYM        "Export"
#define EXPORT   Sll::Plugin::Plugin::InterfaceMap Export()
#define CREATE_PLUGIN_INSTANCE Sll::Plugin::Plugin* __Create(Sll::Object* aParent, const std::string &aId)
#define DELETE_PLUGIN_INSTANCE void __Delete(Sll::Plugin::Plugin* aPlugin)

class Plugin :public Object
{
    std::string _ID;
    friend class DlFile;
public:
    using InterfaceMap = std::map<std::string, FARPROC>;
protected:
    InterfaceMap _Interface;

public:
    Plugin() = delete;
    Plugin(const Plugin&) = delete;
    Plugin(Object* aParent, const std::string& aId);
    ~Plugin() override;


    template<typename _Ret, typename ..._Args> _Ret Proc(const std::string& aProcID, _Args...aArgs)
    {
        using F = _Ret(*)(Plugin*, _Args...aArgs);
        auto f = _Interface.find(aProcID);
        if (f == _Interface.end())
            return _Ret();
        //throw message::push(message::type::error), " - Symbol '", f_id.c_str(), "': not found.";

        F fn = reinterpret_cast<F>(f->second);
        if (fn)
            return fn(this, aArgs...);
    }

    void SetInterface(const InterfaceMap& aXi) { _Interface = aXi; }

    virtual Annotation::Code Run() = 0;
};


class DlFile final : public Object
{
    HINSTANCE _Handle = nullptr;
    std::string _ID;
    static Plugin::InterfaceMap _null;
    Plugin* _Plugin = nullptr;
    Plugin::InterfaceMap _Interface;

public:

    using DllResult = Expect<Plugin*>;

    DlFile() {}
    DlFile(Object* aParent, const std::string& aId);
    ~DlFile() = default;
    std::string FindLocation();
    DllResult Open();
    int Close();
    int Release();
};


*/
