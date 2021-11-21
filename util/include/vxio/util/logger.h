//
// Created by oldlonecoder on 21-11-20.
//

//#ifndef VXIO_FRAMEWORK_LOGGER_H
//#define VXIO_FRAMEWORK_LOGGER_H
#pragma once

#include <vxio/util/rem.h>



class VXIO_UTIL_DLL logger
{
    static rem::list _rems;
public:
    
    
    static rem& debug(const source::location& src = {});
    
    
};



//#endif //VXIO_FRAMEWORK_LOGGER_H
