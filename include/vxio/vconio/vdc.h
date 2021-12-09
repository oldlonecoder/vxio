#pragma once

#include <vxio/vconio/vconio.h>
#include <cstdint>



namespace conio
{




struct VXIO_API_DLL vchar
{
    using type = uint64_t;
    vchar::type cell = 0l;
    
};


}