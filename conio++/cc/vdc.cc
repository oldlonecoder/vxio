//#include <vxio/conio++/vdc.h>
#include <vxio/conio++/conio++.h>

#include <vxio/util/logger.h>
#ifndef _WIN32
#   include <unistd.h>
#endif



namespace vxio::conio
{

uint8_t vch::ascii() const
{
    return aaffbbcc & cc_mask;
}

color::type vch::bg() const
{
    return static_cast<color::type>((aaffbbcc & bb_mask) >> bg_shift);
}

color::type vch::fg() const
{
    return static_cast<color::type>((aaffbbcc & ff_mask) >> fg_shift);
}


uint8_t vch::attr() const
{
    return (aaffbbcc & aa_mask) >> at_shift;
}

vch& vch::set_bg(color::type a_bg)
{
    aaffbbcc &= ~(bb_mask);
    aaffbbcc |= ((uint32_t)a_bg) << bg_shift;
    return *this;
}

vch& vch::set_fg(color::type a_fg)
{
    aaffbbcc &= ~(ff_mask);
    aaffbbcc |= ((uint32_t)a_fg) << fg_shift;
    return *this;
}

vch& vch::set_color(color::type bg_, color::type fg_)
{
    aaffbbcc &= ~(bb_mask);
    aaffbbcc |= ((uint32_t)bg_) << bg_shift;
    aaffbbcc &= ~(ff_mask);
    aaffbbcc |= ((uint32_t)fg_) << fg_shift;
    return *this;
}

vch& vch::set_attr(uint8_t a_attr)
{
    aaffbbcc &= ~(aa_mask);
    aaffbbcc |= ((uint32_t)a_attr) << at_shift;
    return *this;
}

vch& vch::operator=(uint32_t rhs)
{
    aaffbbcc = rhs;
    return *this;
}
vch& vch::operator=(uint8_t a_cc)
{
    aaffbbcc &= ~cc_mask;
    aaffbbcc |= a_cc;
    return *this;
}

vch& vch::operator=(char a_cc)
{
    // auto r = diagnostic::return_debug(src_long_funcname) << diagnostic::funcname << " - char:'" << a_cc << "':\n";
    // std::cout << r.cc();

    aaffbbcc &= ~cc_mask;
    aaffbbcc |= a_cc;
    return *this;
}



//vch& operator=(wchar_t a_cc);
vch& vch::operator=(vch&& rhs) noexcept
{
    aaffbbcc = rhs.aaffbbcc;
    return *this;
}
vch& vch::operator=(const vch& rhs)
= default;




vch& vch::operator>>(std::string& _in)
{
    iostr str = "%s";
    str << color::ansi(fg(), bg()) << ascii();
    _in = str();
    return *this;
}

vch& vch::operator>>(std::ostream& out)
{
    iostr str;
    str << color::ansi(fg(), bg()) << (char)ascii();
    (void)write(1, str().c_str(), str.length());
    //out << color::ansi(fg(), bg()) << (char)ascii();
    return *this;
}


vdc::vdc()
{

}


vdc::vdc(object *parent_, const size &sz_, bool own_):
_parent(parent_), _dim(sz_), _own(own_)
{

}


vdc::~vdc()
{}

rem::code vdc::write(const std::string& txt_)
{
    if(!_bloc) return rem::code::null_ptr;
    
    return rem::code::ok;
}


rem::code vdc::set_geometry(const vxy &size_)
{
    //if(_bloc)
    delete [] _bloc;
    _dim = size_;
    
    _bloc = new vch::type [_dim.wh.y * _dim.wh.x + _dim.wh.x];
    return rem::code::ok;
}


rem::code vdc::init()
{
    
    return rem::code::implement;
}



void vdc::clear(vch attr_)
{
    if(!_bloc)
        return;
    vch::type *p   = _bloc;
    auto      area = _dim.wh.x * _dim.wh.y;
    vch::type *  pend = _bloc + area;
    
    while(p < pend) *p++ = _attr.aaffbbcc;
}



void vdc::commit(const rectangle &r)
{
    vch::type *p =_bloc;
    ts::set_xy(_position);
    ts::put_attr(_attr);
    vch::type a = *p & vch::cmask;
    
    
    for(auto y = 0; y < _dim.wh.y; y++)
    {
        p = _bloc + y * _dim.wh.y;
        for(auto x = 0; x < _dim.wh.x; x++)
        {
            if((*p & vch::cmask) != a)
            {
                a = *p & vch::cmask;
                ts::put_attr({*p});
            }
            ts::put((char)(*p&vch::cc_mask));
        }
    }
}

}
