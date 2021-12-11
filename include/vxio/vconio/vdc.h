#pragma once
#include <vxio/util/iostr.h>
#include <vxio/vconio/vconio.h>
//#include <cstdint>



namespace conio
{




struct VXIO_API_DLL vchar
{
    using type = uint64_t;
    using collection = std::vector<vchar>;
    vchar::type cell = 0l;
    //...

    uint8_t character();
    uint8_t character_attr();
    uint8_t attr();
    uint8_t attr_bits();
    std::string wchar();
    
    //----------------------------------------

};


struct VXIO_API_DLL vdc
{
    using backgrid = std::vector<vchar::collection>;
    vdc::backgrid back_buffer;
    // --- notes:
    // dc->back_buffer[y][x].character();
    // dc->back_buffer[y][x].character_attr();
    // dc->back_buffer[y][x].attr();
    // dc->back_buffer[y][x].attr_bits();
    //...etc...
    //----------------------------------------
    
    
};

}