#pragma once

#include <vxio/vconio/vconio.h>
#include <cstdint>



namespace vxio::conio
{

struct VXIO_API_DLL vchar
{
    using type = uint64_t;
    char::type cell = 0l;
    
};


}