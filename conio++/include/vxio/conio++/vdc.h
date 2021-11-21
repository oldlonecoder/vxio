#pragma once

//#include <vxio/conio++/dll_config.h>
//#include <vxio/util/rem.h>
//#include <vxio/util/geometry.h>


#include <vxio/conio++/conio++.h>

namespace vxio::conio
{

struct CONIO_DLL vch
{
    uint32_t aaffbbcc = 0;
    //wchar_t* unicode = nullptr;

    static const uint32_t cc_mask = (uint32_t)0xFF;
    static const uint32_t bb_mask = (uint32_t)0xFF << 8;
    static const uint32_t ff_mask = (uint32_t)0xFF << 16;
    static const uint32_t aa_mask = (uint32_t)0xFF << 24;
    static const uint32_t bg_shift = 8;
    static const uint32_t fg_shift = 16;
    static const uint32_t at_shift = 24;
    static const uint32_t cmask = vch::bb_mask | vch::ff_mask;


    using cell_type = uint32_t;

    uint8_t ascii();
    color::type bg();
    color::type fg();
    uint8_t attr();

    vch& set_bg(color::type a_bg);
    vch& set_fg(color::type a_fg);
    vch& set_color(color::type bg_, color::type fg_);
    vch& set_attr(uint8_t a_attr);

    vch& operator=(uint32_t rhs);
    vch& operator=(uint8_t a_cc);
    vch& operator=(char a_cc);
    //vch& operator=(wchar_t a_cc);
    vch& operator=(vch&& rhs) noexcept;
    vch& operator=(const vch& rhs);
    vch& operator>>(std::string& _in);
    vch& operator>>(std::ostream& out);
    //   void put_bg();
    //   void put_fg();
    void put_color();
    //   void put_attr();
    static uint32_t fbgmask();

};

/*!
   @brief Video Display Context.

   @note ...
   expect<> ts::cursor_t::operator++()
{
    if(_xy._x>=_area.topright()._x)
    {
        _xy._x = _area.topleft()._x; // 0 or topleft x coord...
        if(_xy._y >= _area.bottomright()._y)
            return diagnostic::oob;
        ++_xy._y;
    }
    else
        ++_xy._x;
    return diagnostic::ok;
}


*/
class CONIO_DLL vdc
{
    vch::cell_type* _bloc = nullptr;  ///< back buffer data
    vxio::size       _bloc_size;       ///< dimensions of the real vdc back buffer. Used to address the line and column into the back buffer.=
    vxio::rectangle _geometry;         ///< Geometry of the confinement relative to the parent.
    vxio::vxy        _bloc_position;   ///< Position into the back buffer. 

    bool             _owner = false;    ///< value is true if this instance owns the data bloc. Value is false otherwise.

    // Local geometry and positions. 
    vxio::vxy        _cxy;  ///< internal cursor oring at {0,0}. 

public:

    vdc();
    ~vdc();

    vxio::vxy& cursor() { return _cxy; }
    vxio::rectangle& geometry() { return _geometry; }

    rem::code write(const std::string& txt_);
};

}
