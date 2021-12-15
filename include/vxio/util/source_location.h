//
// Created by oldlonecoder on 21-11-20.
//



#pragma once

#include <vxio/dll_config.h>


namespace source
{

struct VXIO_API_DLL location
{
    std::string func_name;
    std::string filename;
    int line_no;
};

}

#define src_location {__PRETTY_FUNCTION__, __FILE__, __LINE__}
#define src_long_funcname {__PRETTY_FUNCTION__,"",__LINE__}
#define src_funcname {__FUNCTION__,__FILE__,__LINE__}
#define src_filename {__FUNCTION__,__FILE__,__LINE__}
