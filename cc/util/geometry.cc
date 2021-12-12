//
// Created by oldlonecoder on 21-11-20.
//

#include <vxio/util/geometry.h>

#include <utility>


namespace vxio
{

[[maybe_unused]] vxy vxy::invalid((bool) true);

vxy vxy::nul((bool) true);

vxy::vxy() : x(-1), y(-1), bset(false), nil(true)
{
}

vxy::vxy(int px, int py) : x(px), y(py), bset(true), nil(false)
{

}

vxy::~vxy()
{
}

std::string vxy::to_string() const
{
    std::ostringstream _if;
    _if << '(' << x << ',' << y << ')';
    return _if.str();
}

/*!
 *    \fn vxy::isnull()
 */
bool vxy::isnull()
{
    return !nil;
}

/*!
 *    \fn vxy::invalidate()
 */
void vxy::invalidate()
{
    nil = true;
}

/*!
 *    \fn vxy::enable()
 */
void vxy::enable()
{
    nil = false;
}
size::size()
= default;

size::size(vxy xy, vxy mxy, vxy xxy) :
    wh(std::move(xy)), min(std::move(mxy)), max(std::move(xxy))
{}

size &size::operator=(vxy xy)
{
    wh = xy;
    ///@todo check limits!!!!
    return *this;
}
size &size::set_min(vxy xy)
{
    min = xy;
    return *this;
}
size &size::set_max(vxy xy)
{
    max = xy;
    return *this;
}
std::string size::to_string() const
{
    iostr str = "%dx%d";
    str << wh.x << wh.y;
    return str();
}

rectangle rectangle::nil;
rectangle rectangle::_void;

rectangle::rectangle()
{

}

rectangle::rectangle(const vxy &a_, int w, int h)
{
    a = a_;
    b -= {a.x + w - 1, a.y + h - 1};
    s = {w, h};
}

rectangle::rectangle(const vxy &a_, const size &s_)
{
    s = s_;
    a = a_;
    b = {a.x + s.wh.x - 1, a.y + s.wh.y - 1};
}

rectangle::~rectangle()
{
}

rectangle rectangle::intersection(const rectangle &rec) const
{
    // intersect self with <r>
    rectangle r = rectangle::nil;
    vxy       _a, _b;
    _a = {a.x <= rec.a.x ? rec.a.x : a.x, a.y <= rec.a.y ? rec.a.y : a.y};
    _b = {b.x <= rec.b.x ? b.x : rec.b.x, b.y <= rec.b.y ? b.y : rec.b.y};
    
    bool in = contains(_a) || contains(_b);
    if(!in)
        return r;
    r.assign(_a, _b);
    return r;
}

/*!
 * @brief creates a rectangle from the union between this rectangle and the given one
 * @param rectangle r the given rectangle to make the union with this rectangle
 * @return new rectangle as the result of the union
 */
rectangle rectangle::un(const rectangle &r)
{
    int       xx1, h, yy1, w;
    rectangle rec;
    xx1 = r.a.x < a.x ? r.a.x : a.x;
    w   = r.s.wh.x > s.wh.x ? r.s.wh.x : s.wh.x;
    yy1 = r.a.y < a.y ? r.a.y : a.y;
    h   = r.s.wh.y > s.wh.y ? r.s.wh.y : s.wh.y;
    
    rec.assign(xx1, yy1, w, h);
    return rec;
    
}

void rectangle::resizeto(const vxy &xy)
{
    assign(a.x, a.y, xy.x, xy.y);
}

void rectangle::resize(const vxy &dxy)
{
    assign(a.x, a.y, dxy.x, dxy.y);
}

void rectangle::br_resize(const vxy &dxy)
{
    assign(a.x, a.y, s.wh.x + dxy.x, s.wh.y + dxy.y);
    
}

std::string rectangle::to_string() const
{
    iostr str;
    str << "rect[" << a.x << ", " << a.y << ", : " << s.wh.x << '/' << b.x << ", " << s.wh.y << '/' << b.y << ']';
    return str();
}






//- -------------------------------- winbuffer ------------------------------------------------------------


winbuffer &winbuffer::gotoxy(int x, int y)
{
    cxy = {x, y};
    return *this;
}

void winbuffer::set_geometry(int w, int h)
{
    r  = {{0, 0}, w, h};
    sz = r.s; // sous reserve  : pour fin de limite
    clear();
}

winbuffer &winbuffer::operator++()
{
    if(cxy.x >= r.s.wh.x)
    {
        if(cxy.y <= r.s.wh.x)
        {
            cxy.y++;
            cxy.x = 0;
        }
    }
    else
        cxy.x++;
    
    return *this;
}

winbuffer &winbuffer::operator++(int)
{
    if(cxy.x >= r.s.wh.x)
    {
        if(cxy.y <= r.s.wh.x)
        {
            cxy.y++;
            cxy.x = 0;
        }
    }
    else
        cxy.x++;
    return *this;
}

winbuffer &winbuffer::operator--()
{
    
    return *this;
}

winbuffer &winbuffer::operator--(int)
{
    return *this;
}

/// <summary>
/// Serge Lussier
/// p = 3
/// 10-13 + 3; 
///        *   Serge L*
/// </summary>
/// <param name="txt"></param>
/// <returns></returns>
winbuffer &winbuffer::tput(const std::string &txt)
{
    int line_width = r.width() - cxy.x;
    int ln         = txt.length();
    
    int dx = line_width <= ln ? line_width : ln;
    
    std::string::iterator crs = bmap->_d.begin() + cxy.y * r.width() + cxy.x;
    auto                  p   = txt.begin();
    for(int               x   = 0; x < dx; x++)
        *crs++ = *p++;
    
    cxy.x += line_width;
    
    return *this;
}

void winbuffer::clear()
{
    if(!bmap)
        bmap = new iostr;
    else
        bmap->clear();
    
    bmap->_d = std::string(r.width() * r.height(), ' ');
    //std::cerr << __PRETTY_FUNCTION__ << "contents: '" << bmap->_d << "'\n";
}

void winbuffer::release()
{
    delete bmap;
}

std::string winbuffer::details()
{
    iostr str = "winbuffer details:\n";
    str << r.to_string() << " cursor: " << cxy.to_string();
    return str();
}

winbuffer::operator std::string()
{
    std::string str;
    if(!bmap)
        return "oops! there is no such defined and initialised area!";
    str += '\n';
    for(int l = 0; l < r.height(); l++)
    {
        for(int c = 0; c < r.width(); c++)
            str += *(bmap->begin() + (l * r.width() + c));
        str += '\n';
    }
    return str;
}

}
// kate: indent-mode cstyle; replace-tabs on;


