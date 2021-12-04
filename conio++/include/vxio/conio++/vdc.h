#pragma once


#include <vxio/conio++/dll_config.h>
#include <vxio/util/rem.h>
#include <vxio/util/geometry.h>
#include <vxio/util/object.h>


namespace vxio::conio
{


/**
 * @brief 'Virtual CHar"
 * 
 * @author &copy; 2021, Serge Lussier(oldlonecoder), lussier.serge@gmail.com
 * 
 */
struct CONIO_DLL vch
{
    using type = uint32_t;
    vch::type aaffbbcc = 0;
    //wchar_t* unicode = nullptr;
    
    static const uint32_t cc_mask = (uint32_t)0xFF;
    static const uint32_t bb_mask = (uint32_t)0xFF << 8;
    static const uint32_t ff_mask = (uint32_t)0xFF << 16;
    static const uint32_t aa_mask = (uint32_t)0xFF << 24;
    static const uint32_t bg_shift = 8;
    static const uint32_t fg_shift = 16;
    static const uint32_t at_shift = 24;
    static const uint32_t cmask = vch::bb_mask | vch::ff_mask;



    uint8_t ascii() const;
    color::type bg() const;
    color::type fg() const ;
    uint8_t attr() const;

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

};

/*!
   @brief Virtual Display Context.

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
    //vdc*            _parent = nullptr;
    vch::type *     _bloc = nullptr;  ///< back buffer data
    vxio::size      _dim;       ///< dimensions of the real vdc back buffer. Used to address the line and column into the back buffer.=
    vxio::rectangle _clip;         ///< Geometry of the confinement relative to the parent.
    vxio::vxy       _position = {0,0};   ///< top-left position of this vdc relative to the parent widget or toplevel.

    bool            _own = true;    ///< value is true if this instance owns the data bloc, otherwise value is false for the most confined children widgets.

    // Local geometry and positions. 
    vxio::vxy       _cxy;  ///< internal cursor oring at {0,0}.
    object*         _parent = nullptr;
    vch             _attr;
    
    friend class widget;
public:
    
    vdc();
    vdc(object* parent_, const size& sz_, bool own_ = true);
    
    ~vdc();

    vxio::vxy& cursor() { return _cxy; }
    vxio::rectangle geometry() { return {_position, _dim}; }
    rem::code set_geometry(const vxy& size_);
    rem::code write(const std::string& txt_);
    rem::code init();
    void clear(vch attr_ = {0});
    
    void commit(const rectangle& r);
};


}
