//
// Created by oldlonecoder on 21-11-20.
//

//#ifndef VXIO_FRAMEWORK_GEOMETRY_H
//#define VXIO_FRAMEWORK_GEOMETRY_H
#pragma once
#include <vxio/util/iostr.h>

namespace vxio
{
struct VXIO_UTIL_DLL vxy
{
    using collection = std::vector<vxy>;
    using iterator = collection::iterator;
    using const_iterator = collection::const_iterator;
    
    vxy();
    vxy(const vxy& xy) : x(xy.x), y(xy.y), bset(xy.bset) {}
    vxy( int px, int py );
    
    vxy(vxy&& xy)noexcept { x = xy.x; y = xy.y; }
    explicit vxy ( bool ) {
        x    = 0;
        y    = 0;
        nil  = true;
        bset = false;
    }

    ~vxy();
    
    void operator -= ( const vxy& xy ) {
        if (!xy.bset) return;
        x -= xy.x;
        y -= xy.y;
    }

    void operator += ( const vxy& xy ) {
        x += xy.x;
        y += xy.y;
    }
    vxy operator+ ( const vxy& p ) const  {
        return {p.x + x, p.y + y };
    }

    vxy operator- ( const vxy& p ) const  {
        return {x - p.x, y - p.y };
    }

    void operator() ( int nx, int ny ) {
        x     = nx;
        y    = ny;
        bset = true;
        nil  = false;
    }

    void move ( int dx, int dy ) {
        x += dx;
        y += dy;
    }
    [[nodiscard]] std::string to_string() const;

    bool operator == ( const vxy& xy ) const {
        return (x == xy.x && y == xy.y) && (nil == xy.nil);
    }

    vxy& operator= ( const vxy& xy ) {
        x     = xy.x;
        y    = xy.y;
        bset = xy.bset;
        nil  = !xy.bset;
        return *this;
    }
    vxy& operator= (vxy& xy) {
        x     = xy.x;
        y    = xy.y;
        bset = xy.bset;
        nil  = !xy.bset;
        return *this;
    }


    bool operator >= (const vxy& xy) const{
        return x >= xy.y && y >= xy.y;
    }
    bool operator != ( const vxy& xy )const {
        return xy.x != x || xy.y != y;
    }

    bool operator < ( const vxy& xy ) const {
        return (x < xy.x ) || (y < xy.y );
    }

    bool operator > ( const vxy& xy ) const {
        return (x > xy.x ) || (y > xy.y );
    }
    bool isnull();
    void invalidate();
    void enable();

    /*!
     *   @breif operator boolean to check if this instance is nul ==> unset.
     *   @note boolean op = {x|y == -1}
     */
    bool operator !() const
    {
        return (nil || *this == vxy::nul);
    }

    void operator ++(){
        ++x;
    }

    void operator ++(int){
        ++y;
    }


    vxy& operator=(vxy&& xy)
    {
        x    = std::move(xy.x);
        y    = std::move(xy.y);
        bset = std::move(xy.bset); return *this;
    }
    
    [[maybe_unused]] static vxy invalid;
    static vxy nul;
    //private: -- make data public for faster access to avoid function calls overhead!
    int x{};
    int y{};
    int bset =0;

    bool nil =true;
};


struct VXIO_UTIL_DLL size
{
    vxy min = {1,1};
    vxy max = {-1,-1};
    vxy wh  = {-1,-1};
    
    size();
    size(vxy xy, vxy mxy, vxy xxy);
    
    explicit operator bool() const { return wh.x <=0 || wh.y <=0; }
    size& operator=(const size&)=default;
    size& operator=(vxy xy);
    size& set_min(vxy xy);
    size& set_max(vxy xy);
};




struct VXIO_UTIL_DLL rectangle final
{

    vxy a;
    vxy b;
    size s;
    bool null_instance = true;
    
    using collection = std::vector<rectangle>;
    using iterator   = std::vector<rectangle>::iterator;
    using const_iterator = std::vector<rectangle>::const_iterator;
    //using int_has_t = std::map<int, rectangle>;

    rectangle();
    rectangle ( const vxy& a, int w, int h );
    rectangle ( const vxy& a, const size& _size );
    ~rectangle();
    
    [[nodiscard]] bool valid() const
    {
        return s.operator bool();
    }
    
    void operator()( int a, int b, int c, int d ) {
        assign(a,b,c,d);
    }
    bool operator[](const vxy& xy)const {
        return contains(xy);
    }
    
    rectangle operator +( const rectangle& r) {
        rectangle rr = (*this) + r.a;
        return rr;
    }
    
    rectangle operator +( vxy xy) const{
        return {{a.x + xy.x, a.y + xy.y}, s};
    }
    
    bool operator()(const rectangle* r) const{
        // calling a(&b) is to verify if the entire b is inside this rectangle boundaries.
        return  contains(r->a) && contains(r->b);
    }
    bool operator != (const rectangle& r) const{
        return s.wh != r.s.wh;
    }
    rectangle   operator -( vxy xy) const {
        rectangle r(a, s);
        r -= xy;
        return r;
    }
    
    bool operator >= (const rectangle& r) const{
        return s.wh >= r.s.wh;
    }
    bool operator !() const {
        return  s.operator bool();
    }
    bool operator == ( const rectangle& r ) const
    {
        return a == r.a && b==r.b;
    }
    
    rectangle operator& ( const rectangle& r ) const  {
        return intersection ( r );
    }
    rectangle& operator&= ( const rectangle& r )  {
        *this = intersection ( r );
        return *this;
    }
    rectangle operator | ( const rectangle& r ) {
        return un ( r );
    }
    
    rectangle& operator |= (const rectangle& r) {
        *this = un(r);
        return *this;
    }
    rectangle& operator += ( const rectangle& r ) {
        move ( r.a );
        return *this;
    }
    rectangle& operator -= ( const rectangle& r ) {
        move ( -r.a.x, -r.a.y );
        return *this;
    }
    rectangle& operator -= ( const vxy& xy ) {
        move ( -xy.x, -xy.y );
        return *this;
    }
    rectangle& operator += ( const vxy& xy ) {
        move ( xy );
        return *this;
    }
    bool operator &&(const vxy& xy) const
    {
        return contains(xy);
    }
    
    [[nodiscard]] bool contains ( int px, int py ) const {
        return contains ( {px,py} );
    }
    bool contains ( const vxy& p ) const
    {
        return ( ( p.x >= a.x ) && ( p.x <= b.x ) && ( p.y >= a.y ) && ( p.y <= b.y ) );
    }
    rectangle intersection ( const rectangle& ) const;
    
    void assign ( int a_, int b_, int w_, int h_ )
    {
        a.x = a_;
        a.y = b_;
        b.x = a.x + w_ -1;
        b.y = a.y + h_ -1;
        s = { w_,h_ };
        (void)valid();
    }
    
    void assign ( const vxy& pa, const vxy& pb )
    {
        a = pa;
        b = pb;
        s = { b.x - a.x + 1, b.y - a.y + 1 };
        (void)valid();
    }
    rectangle un ( const rectangle& r );
    
    void assign(const vxy& pa, const size& z)
    {
        a = pa;
        b.x = a.x + z.wh.x -1;
        b.y = a.y + z.wh.y -1;
        s = z;
    }
    
    void resizeto ( const vxy& xy );
    void resize ( const vxy& dxy );
    void br_resize(const vxy& dxy);
    void moveto ( const vxy& xy )
    {
        a.x = xy.x;
        a.y = xy.y;
        b.x =  a.x + s.wh.x -1;
        b.y =  a.y + s.wh.y -1;
    }
    
    void move ( const vxy& dxy )
    {
        a.x += dxy.x;
        a.y += dxy.y;
        b.x += dxy.x;
        b.y += dxy.y;
        // no validation check needed here...
    }
    
    void move ( int dx, int dy ) {
        a.x += dx;
        a.y += dy;
        b.x += dx;
        b.y += dy;
    }
    
    void moveto ( int a, int b ) {
        moveto ( vxy { a,b } );
    }
    
    int height() const  {
        return s.wh.y;
    }
    
    [[nodiscard]] size sz() const  {
        return s;
    }
    
    [[nodiscard]] int width() const {
        return s.wh.x;
    }
    [[nodiscard]] std::string to_string() const;
    
    
    
    void resizecenter ( const vxy& dxy )
    {
        assign(a.x+ (-1*dxy.x), a.y+ (-1*dxy.y), s.wh.x+dxy.x*2, s.wh.y+dxy.y*2);
    }
    
    
    /*!
     *        \fn stdui::rectangle::bottomleft()
     */
    [[nodiscard]] vxy bottomleft() const
    {
        return { a.x, b.y };
    }
    
    /*!
     *        \fn stdui::rectangle::topright()
     */
    [[nodiscard]] vxy topright() const
    {
        return { b.x, a.y };
    }
    
    static rectangle nil;
    static rectangle _void;

};



}

//#endif //VXIO_FRAMEWORK_GEOMETRY_H
