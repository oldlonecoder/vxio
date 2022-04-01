//
// Created by oldlonecoder on 21-11-20.
//

#include <vxio/util/iostr.h>
#ifdef _WIN32
    #include <Winsock2.h> 
#else
    #include <arpa/inet.h>
#endif



using std::string;
static vxio::color::format color_format = vxio::color::format::ansi256;
string  iostr::_default_token_separators = "\\%(){}[]`$#@!;,~?^&<>=+-*/:.";
string  iostr::__nullstr__               = "";

std::ostream &operator<<(std::ostream &in, const iostr &_s)
{
    in << _s();
    return in;
}

iostr::iostr()
{
    // _D has been instanciated here as well...
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = _d.cend();
}

iostr::iostr(const char *a_str)
{
    //SOut << __PRETTY_FUNCTION__ << ":[\"" << a_str << "\"]:\n";
    _d = a_str;
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = _d.cend();
}

iostr::iostr(std::string &&a_str)
{
    std::swap(_d, a_str);
    a_str.clear();
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = _d.cend();
    
}

iostr::iostr(const string &a_str)
{
    //ffnl << a_str << "\n";
    _d = a_str;
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = _d.cend();
    
}

iostr::iostr(const iostr &Str)
{
    //ffnl << Str._str << "\n";
    _d      = Str._d;
    _cursor = Str._cursor;
}

iostr::iostr(iostr &&Str) noexcept
{
    //ffnl << Str._str << "\n";
    std::swap(_d, Str._d);
    _cursor = std::move(Str._cursor);
}

iostr::~iostr()
{
    //ffnl << _str << "\n";
    _d.clear();
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = _d.cend();
    
}



//iostr& iostr::operator=(const iostr& a_str)
//{
//    //ffnl << a_str._str << "\n";
//    _D = a_str._D;
//    _ArgPosition = a_str._ArgPosition;
//    _Precision = a_str._Precision;
//    return *this;
//}

iostr &iostr::operator=(iostr &&a_str) noexcept
{
    //ffnl << a_str._str << "\n";
    std::swap(_d, a_str._d);
    _arg_position = 0;
    _cursor       = std::move(a_str._cursor);
    return *this;
}

bool iostr::operator==(const iostr &a_str) const
{
    
    return _d == a_str._d;
    
}

iostr &iostr::operator=(string &&a_str)
{
    //ffnl << a_str << "\n";
    swap(_d, a_str);
    _arg_position = 0;
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = --_d.end();
    
    return *this;
}

iostr &iostr::operator<<(char c)
{
    if(scan_arg() == std::string::npos)
    {
        _d += c;
        return *this;
    }
    return format<char>(c);
}

iostr &iostr::operator=(const char *a_str)
{
    if(a_str)
        _d = a_str;
    else
        _d        = "";
    _arg_position = 0;
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = _d.end();
    
    return *this;
}

iostr &iostr::operator<<(const string &a_str)
{
    //_scan_next_Arg();
    if(scan_arg() == std::string::npos)
    {
        _d += a_str;
        return *this;
    }
    return format<const std::string &>(a_str);
}

iostr::list_t iostr::to_stdlist(int argc, char **argv)
{
    list_t  args;
    for(int x = 0; x < argc; x++)
    {
        args.push_back(argv[x]);
        
    }
    return args;
}

iostr &iostr::operator<<(const iostr &a_str)
{
    //_scan_next_Arg();
    if(scan_arg() == std::string::npos)
    {
        std::ostringstream os;
        os << a_str();
        _d += os.str();
        return *this;
    }
    return format<const std::string &>(a_str());
}

iostr &iostr::operator<<(const char *a_str)
{
    //_scan_next_Arg();
    if(scan_arg() == std::string::npos)
    {
        std::ostringstream os;
        os.precision(_decimal_precision);
        os << a_str;
        _d += os.str();
        return *this;
    }
    return format<const char *>(a_str);
}

iostr &iostr::operator+=(const iostr &a_atr)
{
    _d += a_atr._d;
    return *this;
}
iostr &iostr::operator+=(const string &a_atr)
{
    _d += a_atr;
    return *this;
    
}

iostr &iostr::operator+=(char c)
{
    _d += c;
    return *this;
}

iostr &iostr::operator+(char c)
{
    _d += c;
    return *this;
}

iostr &iostr::operator+(const iostr &a_atr)
{
    _d += a_atr._d;
    return *this;
}
iostr &iostr::operator+(const string &a_atr)
{
    _d += a_atr;
    return *this;
}

iostr &iostr::operator>>(string &arg_)
{
    arg_ = _d;
    return *this;
}

std::string iostr::make_str(const char *B, const char *E)
{
    std::string Str;
    const char *C = B;
    if((!B) || (!E) || (!*B) || (!*E))
        return Str;
    while(C <= E)
        Str += *C++;
    
    return Str;
}

void iostr::clear()
{
    _d.clear();
    _arg_position = (string::size_type) 0;
    _cursor.start = _cursor.pos = _d.cbegin();
    _cursor.stop  = _d.end();
    
}

std::string iostr::datetime(const std::string &str_fmt)
{
    time_t rawtime;
    struct tm *timeinfo;
    char tmb[180];
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(tmb, 60, str_fmt.c_str(), timeinfo);
    std::string _s = tmb;
    return tmb;
}

string::size_type iostr::scan_arg()
{
    _arg_position = _d.find('%', 0);
    return _arg_position;
}

void iostr::put_arg(const string &aStr)
{
    if(scan_arg() == std::string::npos)
    {
        _d.append(aStr);
        return;
    }
    
    format<const string &>(aStr);
}

// Ce qui fait royalement chier avec les iterateurs des stl, m'est que depuis linenum'iterateur, comment on accede � son conteneur ???????
bool iostr::skip_ws(string::iterator &pos)
{
    if(_cursor.pos == _cursor.stop)
        return false;
    
    if(pos == _d.end()) // aucun moyen de savoir si linenum'it�rateur en est un de notre conteneur "_str" !!
        return false;
    while(isspace(*pos))
        ++pos;
    return true;
}

// Ce qui fait royalement chier avec les iterateurs des stl, m'est que depuis linenum'iterateur, comment on accede � son conteneur ???????
bool iostr::skip_ws(const char *pos)
{
    if(!pos)
        return false;
    while(pos && (*pos && isspace(*pos)))
        ++pos;
    return true;
}

string iostr::word::operator()()
{
    string _s;
    if(start == E)
        _s.insert(_s.begin(), start, E + 1);
    else
        _s.insert(_s.begin(), start, E + 1);
    
    return _s;
}

std::string iostr::word::operator*()
{
    string _s;
    if(start == E)
        _s.insert(_s.begin(), start, E + 1);
    else
        _s.insert(_s.begin(), start, E + 1);
    return _s;
}

[[maybe_unused]] std::string iostr::word::mark() const
{
    iostr                        str;
    std::string::const_iterator cstart = start - position;
    
    int                         l  = 1;
    std::string::const_iterator cc = cstart;
    // localiser le debut de la ligne;
    while(*cc && (cc > cstart) && (*cc != '\n') && (*cc != '\r'))
        --cc;
    // debut de la ligne ou de la source:
    if(cc >= cstart)
    {
        if((*cc == '\n') || (*cc == '\r'))
            ++cc;
        while((cc != SE) && (*cc != '\n') && (*cc != '\r'))
            str += *cc++;
    }
    iostr tstr;
    
    tstr << str << '\n';
    for(int x = 1; x < col; x++)
        tstr << ' ';
    tstr << '^';
    return tstr();
}

void iostr::word::operator++()
{
    
}

void iostr::word::operator++(int)
{
}

std::string iostr::word::location()
{
    iostr str = "(%d,%d)";
    str << line << col;
    return str();
}

string::const_iterator iostr::scan_to(string::const_iterator start, char c) const
{
    string::const_iterator p = start;
    ++p;
    while((p != _d.end()) && (*p != c))
        ++p;
    return p;
}

const char *iostr::scan_to(const char *start, char c) const
{
    const char *p = start;
    if(!p)
        return nullptr;
    ++p;
    while(p && (*p != c))
        ++p;
    return p;
}

/*!
    * @brief break/split/tokenize,etc... the content of this iostr into pieces.
    * @param wcollection  OUTPUT reference to the 'Words array' containter, filled by this method.
    * @param a_delimiters Separators in the form of a string of ascii-8 characters.
    * @param keep_as_word if true (or non-zero), the Separators will be put into the collection as they appear
    * @return number of "Words/tokens" contained into the wcollection.
    * @notice : After several years of experience and experimentations, offset have determined that
    * white-spaces/ctrl or spacing characters are silent and implicit delimiters, in addition to the ones supplied by \c a_delimiters.
    */
std::size_t iostr::words(iostr::word::list_t &wcollection, const std::string &a_delimiters, bool keep_as_word) const
{
    
    iostr::s_p_s Crs = iostr::s_p_s(_d);
    if(_d.empty())
    {
        std::cout << " --> Contents is Empty!";
        return (std::size_t) 0;
    }
    Crs.reset(_d);
    std::string token_separators = a_delimiters.empty() ? iostr::_default_token_separators : a_delimiters;
    if(!Crs.skip())
    {
        //std::cout << " --> Contents Skip is false? (internal?)...\n";
        return (std::size_t) 0;
    }
    word w;
    Crs >> w;
    
    while(!Crs.end())
    {
        //if (!wcollection.empty());
        std::string::const_iterator cc = Crs.pos;
        if(token_separators.find(*Crs.pos) != string::npos)
        {
            cc = Crs.pos;
            if(cc > w.start)
            {
                --cc;
                wcollection.push_back({w.start, cc, Crs.stop, w.line, w.col, w.position});
                
                Crs >> w;
                cc = Crs.pos;
            }
            
            // '//' as one token instead of having two consecutive '/'
            if((*Crs.pos == '/') && (*(Crs.pos + 1) == '/'))
                ++Crs;
            
            if(keep_as_word)
            {
                wcollection.push_back({w.start, Crs.pos, Crs.stop, w.line, w.col, w.position});
            }
            ++Crs;
            //std::cout << "        Iterator eos: " << _Cursor.end() << "\n";
            if(!Crs.end())
                Crs >> w;
            else
            {
                return wcollection.size();
            }
            
        }
        else if((*Crs.pos == '\'') || (*Crs.pos == '"'))
        { // Quoted litteral string...
            Crs >> w;
            if(keep_as_word)
            {
                // Create the three parts of the quoted string: (") + (litteral) + (") ( or ' )
                // So, we save the Word coords anyway.
                wcollection.push_back({w.start, w.start, Crs.stop, w.line, w.col, w.position});
            }
            
            string::const_iterator p = scan_to(w.start + (keep_as_word ? 0 : 1), *Crs.pos); // w.B is the starting position, _Cursor.m is the quote delim.
            while(Crs.pos < p)
                ++Crs; // compute white spaces!!!
            
            if(keep_as_word)
            {
                // then push the litteral that is inside the quotes.
                wcollection.push_back({w.start + 1, p - 1, Crs.stop, w.line, w.col, w.position});
                //++_Cursor; // _Cursor now on the closing quote
                Crs >> w; // Litteral is done, update w.
                wcollection.push_back({w.start, p, Crs.stop, w.line, w.col, w.position});
            }
            else
            {
                // Push the entire quote delims surrounding the litteral as the Word.
                wcollection.push_back({w.start, Crs.pos, Crs.stop, w.line, w.col, w.position});
            }
            if(++Crs)
                Crs >> w;
            else
                return wcollection.size();
            
        }
        else
        {
            cc = Crs.pos;
            ++cc;
            if(cc == Crs.stop)
            {
                ++Crs.pos;
                break;
            }
            if(isspace(*cc))
            {
                if(w.start < cc)
                {
                    wcollection.push_back({w.start, cc - 1, Crs.stop, w.line, w.col, w.position});
                    ++Crs;
                }
                
                if(Crs.skip())
                {
                    Crs >> w;
                    continue;
                }
                return wcollection.size();
            }
            if(!Crs.end())
                ++Crs; // advance offset to the next separator/white space.
        }
    }
    if(Crs.pos > w.start)
        wcollection.push_back({w.start, Crs.pos - 1, Crs.stop, w.line, w.col, w.position});
    
    return wcollection.size();
}

int iostr::filter(const iostr::list_t &a_exp)
{
    if(!a_exp.size())
        return 0;
    auto i = a_exp.cbegin();
    
    // array::front is the init of the match at the beginning of this string.
    // if ...front() is Empty, then there is no required match at the beginning of this string
    // so proceed to the next substring to find.
    // if the last is not Empty, then match the last substring.
    std::string::size_type pos = 0;
    if(!(*i).empty())
    {
        pos = _d.find((*i));
        if(pos != 0) // no match
            return false;
    }
    else;
    ++i;
    auto end = a_exp.cend();
    --end;
    if(!(*i).empty())
    {
        while(i < end)
        {
            if((*i).empty())
            {
                ++i;
                continue;
            }
            pos = _d.find((*i), pos);
            if(pos != std::string::npos)
            {
                ++pos;
                ++i;
                continue;
            }
            return false;
        }
    }
    if(!(*end).empty())
    {
        std::size_t sz = (*end).size();
        pos            = _d.find((*end), pos);
        if(pos != std::string::npos)
        {
            if((pos + sz) != _d.size())
                return false;
        }
    }
    return true;
}

std::string iostr::SizeF(uint64_t sz)
{
    float fsize       = (float) sz;
    iostr  us          = "";
    int   currentunit = 0;
    iostr  units[]     = {"B", "kb", "mb", "gb", "tb"};
    int   unitssize   = sizeof(units) / sizeof(units[0]);
    
    while(fsize > 1024.0 && currentunit < unitssize - 1)
    {
        fsize /= 1024.0;
        currentunit++;
    }
    
    us = std::fixed;
    us += std::setprecision(2);
    us += fsize;
    us += " ";
    us += units[currentunit];
    
    return us;
}
std::string iostr::extract_surrounded(const std::string &first_lhs, const std::string &first_rhs)
{
    std::size_t lhs_pos = _d.find(first_lhs);
    if(lhs_pos == std::string::npos)
        return "";
    std::size_t rhs_pos = _d.find(first_rhs);
    if(rhs_pos == std::string::npos)
        return "";
    
    return _d.substr(lhs_pos, rhs_pos - lhs_pos);
}

/*!
 * @brief Try to extract the "template" type from the \c func_desc that normally shall be given by __PRETTY_FUNCTION__ at the call Location.
 *
 * template parameter assumed to be "T" ...for now.
 *
 * @param func_desc
 *
 * @note This function has no control of the proper format and (function) prototype data.
 * @return the resulting type text
 */
std::string iostr::type_of(string &&func_desc)// , const std::string& _T) -> _T is the class to search.
{
    iostr               text = std::move(func_desc);
    iostr::word::list_t w;
    
    std::size_t count = text.words(w);
    for(auto    s: w)
    {
        std::cout << s();
    }
    std::cout << '\n';
    return "c to implement, please\n";
}

void iostr::process_arg(iostr::lambda_fn_t Fn)
{
    format_t fmt = {_d};
    
    std::string::iterator c = _d.begin() + _arg_position;
    std::string::iterator n, beg, l;
    beg = n = c;
    ++c;
    // %[flag] :
    
    switch(*c)
    {
        case '-':
        case '+':
        case '#':
        case '0':fmt.F = *c++;
            break;
        default:
            //++m;
            break;
    }
    
    n = c;
    // %[width]:
    while((n != _d.end()) && isdigit(*n))
        ++n;
    l = n;
    --n;
    if(n >= c)
    {
        int t = 0;
        while(n >= c)
            fmt.W = fmt.W + (*(n--) - static_cast<uint64_t>('0')) * pow(10, t++);
    }
    else
        fmt.W = 0;
    c = l;
    
    if(*c == '.')
    {
        fmt.R = fmt.P;
        ++c;
        n = c;
        while((n != _d.end()) && isdigit(*n))
            ++n;
        l = n;
        --n;
        int t = 0;
        fmt.R     = 0;
        while(n >= c)
            fmt.R = fmt.R + (*(n--) - static_cast<uint64_t>('0')) * pow(10, t++);
        c = l;
    }
    else
        fmt.R = fmt.P;
    
    //[.precision]
    n = c;
    //% ([Length]) [specifier]
    switch(*c)
    {
        case 'b':fmt.S = *c++;
            break;
        case 'd': // Decimale ou entier
        case 'i':fmt.S = *c++;
            break;
        case 'x':
        case 'X':fmt.S = *c++;
            break;
        case 'f':
        case 'F':
        case 'g':
        case 'G':fmt.S = *c++;
            break;
        case 's':fmt.S = *c++;
    }
    
    fmt.len = c - beg;
    //std::string ff(_D, _ArgPosition, fmt.distance);
    _d.erase(_arg_position, fmt.len);
    _d.insert(_arg_position, Fn(fmt));
    _arg_position = 0;
}

std::string iostr::format_t::operator()()
{
    std::ostringstream str;
    str << "\nFlag    :" << F << "\n";
    str << "Width     :" << (int) W << "\n";
    str << "Precision :" << (int) P << "\n";
    str << "delta     :" << (int) len << "\n";
    str << "spec      :" << (char) S << "\n";
    return str.str();
}

iostr::s_p_s::s_p_s(const std::string &Str)
{
    start = pos = Str.cbegin();
    stop  = Str.cend();
}

bool iostr::s_p_s::skip()
{
    if(end())
        return false;
    
    while(isspace(*pos))
    {
        switch(*pos)
        {
            case 10:
            {
                if((++pos) >= stop)
                    return false;
                if(*pos == 13)
                    ++pos;
                ++line;
                col = 1;
            }
                break;
            case 13:
            {
                if((++pos) >= stop)
                    return false;
                if(*pos == 10)
                    ++pos;
                ++line;
                col = 1;
            }
                break;
            case '\t':++pos;
                ++col;
                break;
            default:++pos;
                ++col;
                break;
        }
    }
    return pos < stop;
}

bool iostr::s_p_s::end()
{
    return pos >= stop;
}

bool iostr::s_p_s::operator++()
{
    if(pos >= stop)
        return false;
    ++pos;
    ++col;
    if(pos >= stop)
        return false;
    return skip();
}

bool iostr::s_p_s::operator++(int)
{
    if(pos >= stop)
        return false;
    ++pos;
    ++col;
    if(pos >= stop)
        return false;
    return skip();
}

iostr::s_p_s &iostr::s_p_s::operator>>(iostr::word &w)
{
    w.start    = pos;
    w.line     = line;
    w.col      = col;
    w.position = index = (uint64_t) (pos - start);
    return *this;
}

iostr &iostr::operator<<(vxio::color::type c)
{
    if(scan_arg() == std::string::npos)
    {
        //std::cerr << __FUNCTION__ << ":\n";
        _d += vxio::color::ansi(c);
        return *this;
    }
    //@todo handle color output format!
    return format<std::string>(vxio::color::ansi(c));
}

//iostr::geometry &iostr::geometry::gotoxy(int x, int y)
//{
//    cxy = {x, y};
//    return *this;
//}
//
//void iostr::geometry::set_geometry(int w, int h)
//{
//    r  = geom::rectangle(0, 0, w, h);
//    sz = r.__s; // sous reserve  : pour fin de limite
//    clear();
//}
//
//iostr::geometry &iostr::geometry::operator++()
//{
//    if(cxy.x >= r.__s._s.x)
//    {
//        if(cxy.y <= r.__s._s.x)
//        {
//            cxy.y++;
//            cxy.x = 0;
//        }
//    }
//    else
//        cxy.x++;
//
//    return *this;
//}
//
//iostr::geometry &iostr::geometry::operator++(int)
//{
//    if(cxy.x >= r.__s._s.x)
//    {
//        if(cxy.y <= r.__s._s.x)
//        {
//            cxy.y++;
//            cxy.x = 0;
//        }
//    }
//    else
//        cxy.x++;
//    return *this;
//}
//
//iostr::geometry &iostr::geometry::operator--()
//{
//
//    return *this;
//}
//
//iostr::geometry &iostr::geometry::operator--(int)
//{
//    return *this;
//}
//
///// <summary>
///// Serge Lussier
///// p = 3
///// 10-13 + 3;
/////        *   Serge L*
///// </summary>
///// <param name="txt"></param>
///// <returns></returns>
//iostr::geometry &iostr::geometry::tput(const std::string &txt)
//{
//    int line_width = r.width() - cxy.x;
//    int ln         = txt.length();
//
//    int dx = line_width <= ln ? line_width : ln;
//
//    std::string::iterator crs = bmap->_d.begin() + cxy.y * r.width() + cxy.x;
//    auto                  p   = txt.begin();
//    for(int               x   = 0; x < dx; x++)
//        *crs++ = *p++;
//
//    cxy.x += line_width;
//
//    return *this;
//}
//
//void iostr::geometry::clear()
//{
//    if(!bmap)
//        bmap = new iostr;
//    else
//        bmap->clear();
//
//    bmap->_d = std::string(r.width() * r.height(), ' ');
//    //std::cerr << __PRETTY_FUNCTION__ << "contents: '" << bmap->_d << "'\n";
//}
//
//void iostr::geometry::release()
//{
//    if(bmap)
//        delete bmap;
//}
//
//std::string iostr::geometry::details()
//{
//    iostr str = "geometry details:\n";
//    str << r.to_string() << " cursor: " << cxy.to_string();
//    return str();
//}
//
//iostr::geometry::operator std::string()
//{
//    std::string str;
//    if(!bmap)
//        return "oops! there is no such defined and initialised area!";
//    str += '\n';
//    for(int l = 0; l < r.height(); l++)
//    {
//        for(int c = 0; c < r.width(); c++)
//            str += *(bmap->begin() + (l * r.width() + c));
//        str += '\n';
//    }
//    return str;
//}

static std::vector<std::string> html_colors =
{
    /*0   */ "#000"//#000000	rgb(0"0"0)	hsl(0"100%"25%)                       ,
    /*1   */ "#800000"//#800000	rgb(128"0"0)	hsl(0"100%"25%)                       ,
    /*2   */ "Green"//#008000	rgb(0"128"0)	hsl(120"100%"25%)                     ,
    /*3   */ "Olive"//#808000	rgb(128"128"0)	hsl(60"100%"25%)                      ,
    /*4   */ "Navy"//#000080	rgb(0"0"128)	hsl(240"100%"25%)                     ,
    /*5   */ "Purple"//#800080	rgb(128"0"128)	hsl(300"100%"25%)                     ,
    /*6   */ "Teal"//#008080	rgb(0"128"128)	hsl(180"100%"25%)                     ,
    /*7   */ "Silver"//#c0c0c0	rgb(192"192"192)	hsl(0"0%"75%)                     ,
    /*8   */ "Grey"//#808080	rgb(128"128"128)	hsl(0"0%"50%)                     ,
    /*9   */ "Red"//#ff0000	rgb(255"0"0)	hsl(0"100%"50%)                           ,
    /*10  */ "Lime"//#00ff00	rgb(0"255"0)	hsl(120"100%"50%)                     ,
    /*11  */ "Yellow"//#ffff00	rgb(255"255"0)	hsl(60"100%"50%)                      ,
    /*12  */ "Blue"//#0000ff	rgb(0"0"255)	hsl(240"100%"50%)                     ,
    /*13  */ "Fuchsia"//#ff00ff	rgb(255"0"255)	hsl(300"100%"50%)                     ,
    /*14  */ "Aqua"//#00ffff	rgb(0"255"255)	hsl(180"100%"50%)                     ,
    /*15  */ "White"//#ffffff	rgb(255"255"255)	hsl(0"0%"100%)                    ,
    /*16  */ "Grey0"//#000000	rgb(0"0"0)	hsl(0"0%"0%)                              ,
    /*17  */ "NavyBlue"//#00005f	rgb(0"0"95)	hsl(240"100%"18%)                     ,
    /*18  */ "DarkBlue"//#000087	rgb(0"0"135)	hsl(240"100%"26%)                 ,
    /*19  */ "Blue3"//#0000af	rgb(0"0"175)	hsl(240"100%"34%)                     ,
    /*20  */ "Blue4"//#0000d7	rgb(0"0"215)	hsl(240"100%"42%)                     ,
    /*21  */ "Blue1"//#0000ff	rgb(0"0"255)	hsl(240"100%"50%)                     ,
    /*22  */ "DarkGreen"//#005f00	rgb(0"95"0)	hsl(120"100%"18%)                     ,
    /*23  */ "DeepSkyBlue4"//#005f5f	rgb(0"95"95)	hsl(180"100%"18%)             ,
    /*24  */ "DeepSkyBlue5"//#005f87	rgb(0"95"135)	hsl(97"100%"26%)              ,
    /*25  */ "DeepSkyBlue6"//#005faf	rgb(0"95"175)	hsl(07"100%"34%)              ,
    /*26  */ "DodgerBlue3"//#005fd7	rgb(0"95"215)	hsl(13"100%"42%)                  ,
    /*27  */ "DodgerBlue2"//#005fff	rgb(0"95"255)	hsl(17"100%"50%)                  ,
    /*28  */ "Green4"//#008700	rgb(0"135"0)	hsl(120"100%"26%)                     ,
    /*29  */ "SpringGreen4"//#00875f	rgb(0"135"95)	hsl(62"100%"26%)              ,
    /*30  */ "Turquoise4"//#008787	rgb(0"135"135)	hsl(180"100%"26%)                 ,
    /*31  */ "DeepSkyBlue3"//#0087af	rgb(0"135"175)	hsl(93"100%"34%)              ,
    /*32  */ "DeepSkyBlue7"//#0087d7	rgb(0"135"215)	hsl(02"100%"42%)              ,
    /*33  */ "DodgerBlue1"//#0087ff	rgb(0"135"255)	hsl(08"100%"50%)                  ,
    /*34  */ "Green3"//#00af00	rgb(0"175"0)	hsl(120"100%"34%)                     ,
    /*35  */ "SpringGreen3"//#00af5f	rgb(0"175"95)	hsl(52"100%"34%)              ,
    /*36  */ "DarkCyan"//#00af87	rgb(0"175"135)	hsl(66"100%"34%)                  ,
    /*37  */ "LightSeaGreen"//#00afaf	rgb(0"175"175)	hsl(180"100%"34%)             ,
    /*38  */ "DeepSkyBlue2"//#00afd7	rgb(0"175"215)	hsl(91"100%"42%)              ,
    /*39  */ "DeepSkyBlue1"//#00afff	rgb(0"175"255)	hsl(98"100%"50%)              ,
    /*40  */ "Green5"//#00d700	rgb(0"215"0)	hsl(120"100%"42%)                     ,
    /*41  */ "SpringGreen5"//#00d75f	rgb(0"215"95)	hsl(46"100%"42%)              ,
    /*42  */ "SpringGreen2"//#00d787	rgb(0"215"135)	hsl(57"100%"42%)              ,
    /*43  */ "Cyan3"//#00d7af	rgb(0"215"175)	hsl(68"100%"42%)                      ,
    /*44  */ "DarkTurquoise"//#00d7d7	rgb(0"215"215)	hsl(180"100%"42%)             ,
    /*45  */ "Turquoise2"//#00d7ff	rgb(0"215"255)	hsl(89"100%"50%)                  ,
    /*46  */ "Green1"//#00ff00	rgb(0"255"0)	hsl(120"100%"50%)                     ,
    /*47  */ "SpringGreen6"//#00ff5f	rgb(0"255"95)	hsl(42"100%"50%)              ,
    /*48  */ "SpringGreen1"//#00ff87	rgb(0"255"135)	hsl(51"100%"50%)              ,
    /*49  */ "MediumSpringGreen"//#00ffaf	rgb(0"255"175)	hsl(61"100%"50%)          ,
    /*50  */ "Cyan2"//#00ffd7	rgb(0"255"215)	hsl(70"100%"50%)                      ,
    /*51  */ "Cyan1"//#00ffff	rgb(0"255"255)	hsl(180"100%"50%)                     ,
    /*52  */ "DarkRed"//#5f0000	rgb(95"0"0)	hsl(0"100%"18%)                           ,
    /*53  */ "DeepPink4"//#5f005f	rgb(95"0"95)	hsl(300"100%"18%)                 ,
    /*54  */ "Purple4"//#5f0087	rgb(95"0"135)	hsl(82"100%"26%)                      ,
    /*55  */ "Purple5"//#5f00af	rgb(95"0"175)	hsl(72"100%"34%)                      ,
    /*56  */ "Purple3"//#5f00d7	rgb(95"0"215)	hsl(66"100%"42%)                      ,
    /*57  */ "BlueViolet"//#5f00ff	rgb(95"0"255)	hsl(62"100%"50%)                  ,
    /*58  */ "Orange4"//#5f5f00	rgb(95"95"0)	hsl(60"100%"18%)                      ,
    /*59  */ "Grey37"//#5f5f5f	rgb(95"95"95)	hsl(0"0%"37%)                         ,
    /*60  */ "MediumPurple4"//#5f5f87	rgb(95"95"135)	hsl(240"17%"45%)              ,
    /*61  */ "SlateBlue3"//#5f5faf	rgb(95"95"175)	hsl(240"33%"52%)                  ,
    /*62  */ "SlateBlue4"//#5f5fd7	rgb(95"95"215)	hsl(240"60%"60%)                  ,
    /*63  */ "RoyalBlue1"//#5f5fff	rgb(95"95"255)	hsl(240"100%"68%)                 ,
    /*64  */ "Chartreuse4"//#5f8700	rgb(95"135"0)	hsl(7"100%"26%)                   ,
    /*65  */ "DarkSeaGreen4"//#5f875f	rgb(95"135"95)	hsl(120"17%"45%)              ,
    /*66  */ "PaleTurquoise4"//#5f8787	rgb(95"135"135)	hsl(180"17%"45%)              ,
    /*67  */ "SteelBlue"//#5f87af	rgb(95"135"175)	hsl(210"33%"52%)                  ,
    /*68  */ "SteelBlue3"//#5f87d7	rgb(95"135"215)	hsl(220"60%"60%)                  ,
    /*69  */ "CornflowerBlue"//#5f87ff	rgb(95"135"255)	hsl(225"100%"68%)             ,
    /*70  */ "Chartreuse3"//#5faf00	rgb(95"175"0)	hsl(7"100%"34%)                   ,
    /*71  */ "DarkSeaGreen5"//#5faf5f	rgb(95"175"95)	hsl(120"33%"52%)              ,
    /*72  */ "CadetBlue"//#5faf87	rgb(95"175"135)	hsl(150"33%"52%)                  ,
    /*73  */ "CadetBlue2"//#5fafaf	rgb(95"175"175)	hsl(180"33%"52%)                  ,
    /*74  */ "SkyBlue3"//#5fafd7	rgb(95"175"215)	hsl(200"60%"60%)                  ,
    /*75  */ "SteelBlue1"//#5fafff	rgb(95"175"255)	hsl(210"100%"68%)                 ,
    /*76  */ "Chartreuse5"//#5fd700	rgb(95"215"0)	hsl(3"100%"42%)                   ,
    /*77  */ "PaleGreen3"//#5fd75f	rgb(95"215"95)	hsl(120"60%"60%)                  ,
    /*78  */ "SeaGreen3"//#5fd787	rgb(95"215"135)	hsl(140"60%"60%)                  ,
    /*79  */ "Aquamarine3"//#5fd7af	rgb(95"215"175)	hsl(160"60%"60%)                  ,
    /*80  */ "MediumTurquoise"//#5fd7d7	rgb(95"215"215)	hsl(180"60%"60%)              ,
    /*81  */ "SteelBlue4"//#5fd7ff	rgb(95"215"255)	hsl(195"100%"68%)                 ,
    /*82  */ "Chartreuse2"//#5fff00	rgb(95"255"0)	hsl(7"100%"50%)                   ,
    /*83  */ "SeaGreen2"//#5fff5f	rgb(95"255"95)	hsl(120"100%"68%)                 ,
    /*84  */ "SeaGreen1"//#5fff87	rgb(95"255"135)	hsl(135"100%"68%)                 ,
    /*85  */ "SeaGreen4"//#5fffaf	rgb(95"255"175)	hsl(150"100%"68%)                 ,
    /*86  */ "Aquamarine1"//#5fffd7	rgb(95"255"215)	hsl(165"100%"68%)                 ,
    /*87  */ "DarkSlateGray2"//#5fffff	rgb(95"255"255)	hsl(180"100%"68%)             ,
    /*88  */ "DarkRed2"//#870000	rgb(135"0"0)	hsl(0"100%"26%)                   ,
    /*89  */ "DeepPink5"//#87005f	rgb(135"0"95)	hsl(17"100%"26%)                  ,
    /*90  */ "DarkMagenta"//#870087	rgb(135"0"135)	hsl(300"100%"26%)                 ,
    /*91  */ "DarkMagenta2"//#8700af	rgb(135"0"175)	hsl(86"100%"34%)              ,
    /*92  */ "DarkViolet"//#8700d7	rgb(135"0"215)	hsl(77"100%"42%)                  ,
    /*93  */ "Purple2"//#8700ff	rgb(135"0"255)	hsl(71"100%"50%)                      ,
    /*94  */ "Orange5"//#875f00	rgb(135"95"0)	hsl(2"100%"26%)                       ,
    /*95  */ "LightPink4"//#875f5f	rgb(135"95"95)	hsl(0"17%"45%)                    ,
    /*96  */ "Plum4"//#875f87	rgb(135"95"135)	hsl(300"17%"45%)                      ,
    /*97  */ "MediumPurple3"//#875faf	rgb(135"95"175)	hsl(270"33%"52%)              ,
    /*98  */ "MediumPurple5"//#875fd7	rgb(135"95"215)	hsl(260"60%"60%)              ,
    /*99  */ "SlateBlue1"//#875fff	rgb(135"95"255)	hsl(255"100%"68%)                 ,
    /*100 */ "Yellow4"//#878700	rgb(135"135"0)	hsl(60"100%"26%)                      ,
    /*101 */ "Wheat4"//#87875f	rgb(135"135"95)	hsl(60"17%"45%)                       ,
    /*102 */ "Grey53"//#878787	rgb(135"135"135)	hsl(0"0%"52%)                     ,
    /*103 */ "LightSlateGrey"//#8787af	rgb(135"135"175)	hsl(240"20%"60%)          ,
    /*104 */ "MediumPurple"//#8787d7	rgb(135"135"215)	hsl(240"50%"68%)          ,
    /*105 */ "LightSlateBlue"//#8787ff	rgb(135"135"255)	hsl(240"100%"76%)         ,
    /*106 */ "Yellow5"//#87af00	rgb(135"175"0)	hsl(3"100%"34%)                       ,
    /*107 */ "DarkOliveGreen3"//#87af5f	rgb(135"175"95)	hsl(90"33%"52%)               ,
    /*108 */ "DarkSeaGreen"//#87af87	rgb(135"175"135)	hsl(120"20%"60%)          ,
    /*109 */ "LightSkyBlue3"//#87afaf	rgb(135"175"175)	hsl(180"20%"60%)          ,
    /*110 */ "LightSkyBlue4"//#87afd7	rgb(135"175"215)	hsl(210"50%"68%)          ,
    /*111 */ "SkyBlue2"//#87afff	rgb(135"175"255)	hsl(220"100%"76%)             ,
    /*112 */ "Chartreuse6"//#87d700	rgb(135"215"0)	hsl(2"100%"42%)                   ,
    /*113 */ "DarkOliveGreen4"//#87d75f	rgb(135"215"95)	hsl(100"60%"60%)              ,
    /*114 */ "PaleGreen4"//#87d787	rgb(135"215"135)	hsl(120"50%"68%)              ,
    /*115 */ "DarkSeaGreen3"//#87d7af	rgb(135"215"175)	hsl(150"50%"68%)          ,
    /*116 */ "DarkSlateGray3"//#87d7d7	rgb(135"215"215)	hsl(180"50%"68%)          ,
    /*117 */ "SkyBlue1"//#87d7ff	rgb(135"215"255)	hsl(200"100%"76%)             ,
    /*118 */ "Chartreuse1"//#87ff00	rgb(135"255"0)	hsl(8"100%"50%)                   ,
    /*119 */ "LightGreen"//#87ff5f	rgb(135"255"95)	hsl(105"100%"68%)                 ,
    /*120 */ "LightGreen2"//#87ff87	rgb(135"255"135)	hsl(120"100%"76%)             ,
    /*121 */ "PaleGreen1"//#87ffaf	rgb(135"255"175)	hsl(140"100%"76%)             ,
    /*122 */ "Aquamarine2"//#87ffd7	rgb(135"255"215)	hsl(160"100%"76%)             ,
    /*123 */ "DarkSlateGray1"//#87ffff	rgb(135"255"255)	hsl(180"100%"76%)         ,
    /*124 */ "Red3"//#af0000	rgb(175"0"0)	hsl(0"100%"34%)                       ,
    /*125 */ "DeepPink6"//#af005f	rgb(175"0"95)	hsl(27"100%"34%)                  ,
    /*126 */ "MediumVioletRed"//#af0087	rgb(175"0"135)	hsl(13"100%"34%)              ,
    /*127 */ "Magenta3"//#af00af	rgb(175"0"175)	hsl(300"100%"34%)                 ,
    /*128 */ "DarkViole2"//#af00d7	rgb(175"0"215)	hsl(88"100%"42%)                  ,
    /*129 */ "Purple6"//#af00ff	rgb(175"0"255)	hsl(81"100%"50%)                      ,
    /*130 */ "DarkOrange3"//#af5f00	rgb(175"95"0)	hsl(2"100%"34%)                   ,
    /*131 */ "IndianRed"//#af5f5f	rgb(175"95"95)	hsl(0"33%"52%)                    ,
    /*132 */ "HotPink3"//#af5f87	rgb(175"95"135)	hsl(330"33%"52%)                  ,
    /*133 */ "MediumOrchid3"//#af5faf	rgb(175"95"175)	hsl(300"33%"52%)              ,
    /*134 */ "MediumOrchid"//#af5fd7	rgb(175"95"215)	hsl(280"60%"60%)              ,
    /*135 */ "MediumPurple2"//#af5fff	rgb(175"95"255)	hsl(270"100%"68%)             ,
    /*136 */ "DarkGoldenrod"//#af8700	rgb(175"135"0)	hsl(6"100%"34%)               ,
    /*137 */ "LightSalmon3"//#af875f	rgb(175"135"95)	hsl(30"33%"52%)               ,
    /*138 */ "RosyBrown"//#af8787	rgb(175"135"135)	hsl(0"20%"60%)                ,
    /*139 */ "Grey63"//#af87af	rgb(175"135"175)	hsl(300"20%"60%)                  ,
    /*140 */ "MediumPurple6"//#af87d7	rgb(175"135"215)	hsl(270"50%"68%)          ,
    /*141 */ "MediumPurple1"//#af87ff	rgb(175"135"255)	hsl(260"100%"76%)         ,
    /*142 */ "Gold3"//#afaf00	rgb(175"175"0)	hsl(60"100%"34%)                      ,
    /*143 */ "DarkKhaki"//#afaf5f	rgb(175"175"95)	hsl(60"33%"52%)                   ,
    /*144 */ "NavajoWhite3"//#afaf87	rgb(175"175"135)	hsl(60"20%"60%)           ,
    /*145 */ "Grey69"//#afafaf	rgb(175"175"175)	hsl(0"0%"68%)                     ,
    /*146 */ "LightSteelBlue3"//#afafd7	rgb(175"175"215)	hsl(240"33%"76%)          ,
    /*147 */ "LightSteelBlue"//#afafff	rgb(175"175"255)	hsl(240"100%"84%)         ,
    /*148 */ "Yellow3"//#afd700	rgb(175"215"0)	hsl(1"100%"42%)                       ,
    /*149 */ "DarkOliveGreen5"//#afd75f	rgb(175"215"95)	hsl(80"60%"60%)               ,
    /*150 */ "DarkSeaGreen6"//#afd787	rgb(175"215"135)	hsl(90"50%"68%)           ,
    /*151 */ "DarkSeaGreen2"//#afd7af	rgb(175"215"175)	hsl(120"33%"76%)          ,
    /*152 */ "LightCyan3"//#afd7d7	rgb(175"215"215)	hsl(180"33%"76%)              ,
    /*153 */ "LightSkyBlue1"//#afd7ff	rgb(175"215"255)	hsl(210"100%"84%)         ,
    /*154 */ "GreenYellow"//#afff00	rgb(175"255"0)	hsl(8"100%"50%)                   ,
    /*155 */ "DarkOliveGreen2"//#afff5f	rgb(175"255"95)	hsl(90"100%"68%)              ,
    /*156 */ "PaleGreen2"//#afff87	rgb(175"255"135)	hsl(100"100%"76%)             ,
    /*157 */ "DarkSeaGreen7"//#afffaf	rgb(175"255"175)	hsl(120"100%"84%)         ,
    /*158 */ "DarkSeaGreen1"//#afffd7	rgb(175"255"215)	hsl(150"100%"84%)         ,
    /*159 */ "PaleTurquoise1"//#afffff	rgb(175"255"255)	hsl(180"100%"84%)         ,
    /*160 */ "Red4"//#d70000	rgb(215"0"0)	hsl(0"100%"42%)                       ,
    /*161 */ "DeepPink3"//#d7005f	rgb(215"0"95)	hsl(33"100%"42%)                  ,
    /*162 */ "DeepPink7"//#d70087	rgb(215"0"135)	hsl(22"100%"42%)                  ,
    /*163 */ "Magenta4"//#d700af	rgb(215"0"175)	hsl(11"100%"42%)                  ,
    /*164 */ "Magenta5"//#d700d7	rgb(215"0"215)	hsl(300"100%"42%)                 ,
    /*165 */ "Magenta2"//#d700ff	rgb(215"0"255)	hsl(90"100%"50%)                  ,
    /*166 */ "DarkOrange4"//#d75f00	rgb(215"95"0)	hsl(6"100%"42%)                   ,
    /*167 */ "IndianRed2"//#d75f5f	rgb(215"95"95)	hsl(0"60%"60%)                    ,
    /*168 */ "HotPink4"//#d75f87	rgb(215"95"135)	hsl(340"60%"60%)                  ,
    /*169 */ "HotPink2"//#d75faf	rgb(215"95"175)	hsl(320"60%"60%)                  ,
    /*170 */ "Orchid"//#d75fd7	rgb(215"95"215)	hsl(300"60%"60%)                      ,
    /*171 */ "MediumOrchid1"//#d75fff	rgb(215"95"255)	hsl(285"100%"68%)             ,
    /*172 */ "Orange3"//#d78700	rgb(215"135"0)	hsl(7"100%"42%)                       ,
    /*173 */ "LightSalmon4"//#d7875f	rgb(215"135"95)	hsl(20"60%"60%)               ,
    /*174 */ "LightPink3"//#d78787	rgb(215"135"135)	hsl(0"50%"68%)                ,
    /*175 */ "Pink3"//#d787af	rgb(215"135"175)	hsl(330"50%"68%)                  ,
    /*176 */ "Plum3"//#d787d7	rgb(215"135"215)	hsl(300"50%"68%)                  ,
    /*177 */ "Violet"//#d787ff	rgb(215"135"255)	hsl(280"100%"76%)                 ,
    /*178 */ "Gold4"//#d7af00	rgb(215"175"0)	hsl(8"100%"42%)                       ,
    /*179 */ "LightGoldenrod3"//#d7af5f	rgb(215"175"95)	hsl(40"60%"60%)               ,
    /*180 */ "Tan"//#d7af87	rgb(215"175"135)	hsl(30"50%"68%)                       ,
    /*181 */ "MistyRose3"//#d7afaf	rgb(215"175"175)	hsl(0"33%"76%)                ,
    /*182 */ "Thistle3"//#d7afd7	rgb(215"175"215)	hsl(300"33%"76%)              ,
    /*183 */ "Plum2"//#d7afff	rgb(215"175"255)	hsl(270"100%"84%)                 ,
    /*184 */ "Yellow6"//#d7d700	rgb(215"215"0)	hsl(60"100%"42%)                      ,
    /*185 */ "Khaki3"//#d7d75f	rgb(215"215"95)	hsl(60"60%"60%)                       ,
    /*186 */ "LightGoldenrod2"//#d7d787	rgb(215"215"135)	hsl(60"50%"68%)           ,
    /*187 */ "LightYellow3"//#d7d7af	rgb(215"215"175)	hsl(60"33%"76%)           ,
    /*188 */ "Grey84"//#d7d7d7	rgb(215"215"215)	hsl(0"0%"84%)                     ,
    /*189 */ "LightSteelBlue1"//#d7d7ff	rgb(215"215"255)	hsl(240"100%"92%)         ,
    /*190 */ "Yellow2"//#d7ff00	rgb(215"255"0)	hsl(9"100%"50%)                       ,
    /*191 */ "DarkOliveGreen1"//#d7ff5f	rgb(215"255"95)	hsl(75"100%"68%)              ,
    /*192 */ "DarkOliveGreen6"//#d7ff87	rgb(215"255"135)	hsl(80"100%"76%)          ,
    /*193 */ "DarkSeaGreen8"//#d7ffaf	rgb(215"255"175)	hsl(90"100%"84%)          ,
    /*194 */ "Honeydew2"//#d7ffd7	rgb(215"255"215)	hsl(120"100%"92%)             ,
    /*195 */ "LightCyan1"//#d7ffff	rgb(215"255"255)	hsl(180"100%"92%)             ,
    /*196 */ "Red1"//#ff0000	rgb(255"0"0)	hsl(0"100%"50%)                       ,
    /*197 */ "DeepPink2"//#ff005f	rgb(255"0"95)	hsl(37"100%"50%)                  ,
    /*198 */ "DeepPink1"//#ff0087	rgb(255"0"135)	hsl(28"100%"50%)                  ,
    /*199 */ "DeepPink8"//#ff00af	rgb(255"0"175)	hsl(18"100%"50%)                  ,
    /*200 */ "Magenta6"//#ff00d7	rgb(255"0"215)	hsl(09"100%"50%)                  ,
    /*201 */ "Magenta1"//#ff00ff	rgb(255"0"255)	hsl(300"100%"50%)                 ,
    /*202 */ "OrangeRed1"//#ff5f00	rgb(255"95"0)	hsl(2"100%"50%)                   ,
    /*203 */ "IndianRed1"//#ff5f5f	rgb(255"95"95)	hsl(0"100%"68%)                   ,
    /*204 */ "IndianRed3"//#ff5f87	rgb(255"95"135)	hsl(345"100%"68%)                 ,
    /*205 */ "HotPink"//#ff5faf	rgb(255"95"175)	hsl(330"100%"68%)                     ,
    /*206 */ "HotPin2"//#ff5fd7	rgb(255"95"215)	hsl(315"100%"68%)                     ,
    /*207 */ "MediumOrchid2"//#ff5fff	rgb(255"95"255)	hsl(300"100%"68%)             ,
    /*208 */ "DarkOrange"//#ff8700	rgb(255"135"0)	hsl(1"100%"50%)                   ,
    /*209 */ "Salmon1"//#ff875f	rgb(255"135"95)	hsl(15"100%"68%)                      ,
    /*210 */ "LightCoral"//#ff8787	rgb(255"135"135)	hsl(0"100%"76%)               ,
    /*211 */ "PaleVioletRed1"//#ff87af	rgb(255"135"175)	hsl(340"100%"76%)         ,
    /*212 */ "Orchid2"//#ff87d7	rgb(255"135"215)	hsl(320"100%"76%)                 ,
    /*213 */ "Orchid1"//#ff87ff	rgb(255"135"255)	hsl(300"100%"76%)                 ,
    /*214 */ "Orange1"//#ffaf00	rgb(255"175"0)	hsl(1"100%"50%)                       ,
    /*215 */ "SandyBrown"//#ffaf5f	rgb(255"175"95)	hsl(30"100%"68%)                  ,
    /*216 */ "LightSalmon1"//#ffaf87	rgb(255"175"135)	hsl(20"100%"76%)          ,
    /*217 */ "LightPink1"//#ffafaf	rgb(255"175"175)	hsl(0"100%"84%)               ,
    /*218 */ "Pink1"//#ffafd7	rgb(255"175"215)	hsl(330"100%"84%)                 ,
    /*219 */ "Plum1"//#ffafff	rgb(255"175"255)	hsl(300"100%"84%)                 ,
    /*220 */ "Gold1"//#ffd700	rgb(255"215"0)	hsl(0"100%"50%)                       ,
    /*221 */ "LightGoldenrod4"//#ffd75f	rgb(255"215"95)	hsl(45"100%"68%)              ,
    /*222 */ "LightGoldenrod5"//#ffd787	rgb(255"215"135)	hsl(40"100%"76%)          ,
    /*223 */ "NavajoWhite1"//#ffd7af	rgb(255"215"175)	hsl(30"100%"84%)          ,
    /*224 */ "MistyRose1"//#ffd7d7	rgb(255"215"215)	hsl(0"100%"92%)               ,
    /*225 */ "Thistle1"//#ffd7ff	rgb(255"215"255)	hsl(300"100%"92%)             ,
    /*226 */ "Yellow1"//#ffff00	rgb(255"255"0)	hsl(60"100%"50%)                      ,
    /*227 */ "LightGoldenrod1"//#ffff5f	rgb(255"255"95)	hsl(60"100%"68%)              ,
    /*228 */ "Khaki1"//#ffff87	rgb(255"255"135)	hsl(60"100%"76%)                  ,
    /*229 */ "Wheat1"//#ffffaf	rgb(255"255"175)	hsl(60"100%"84%)                  ,
    /*230 */ "Cornsilk1"//#ffffd7	rgb(255"255"215)	hsl(60"100%"92%)              ,
    /*231 */ "Grey100"//#ffffff	rgb(255"255"255)	hsl(0"0%"100%)                    ,
    /*232 */ "Grey3"//#080808	rgb(8"8"8)	hsl(0"0%"3%)                              ,
    /*233 */ "Grey7"//#121212	rgb(18"18"18)	hsl(0"0%"7%)                          ,
    /*234 */ "Grey11"//#1c1c1c	rgb(28"28"28)	hsl(0"0%"10%)                         ,
    /*235 */ "Grey15"//#262626	rgb(38"38"38)                                         ,
    /*236 */ "Grey19"//#303030	rgb(48"48"48)	hsl(0"0%"18%)                         ,
    /*237 */ "Grey23"//#3a3a3a	rgb(58"58"58)	hsl(0"0%"22%)                         ,
    /*238 */ "Grey27"//#444444	rgb(68"68"68)	hsl(0"0%"26%)                         ,
    /*239 */ "Grey30"//#4e4e4e	rgb(78"78"78)	hsl(0"0%"30%)                         ,
    /*240 */ "Grey35"//#585858	rgb(88"88"88)	hsl(0"0%"34%)                         ,
    /*241 */ "Grey39"//#626262	rgb(98"98"98)	hsl(0"0%"37%)                         ,
    /*242 */ "Grey42"//#6c6c6c	rgb(108"108"108)	hsl(0"0%"40%)                     ,
    /*243 */ "Grey46"//#767676	rgb(118"118"118)	hsl(0"0%"46%)                     ,
    /*244 */ "Grey50"//#808080	rgb(128"128"128)	hsl(0"0%"50%)                     ,
    /*245 */ "Grey54"//#8a8a8a	rgb(138"138"138)	hsl(0"0%"54%)                     ,
    /*246 */ "Grey58"//#949494	rgb(148"148"148)	hsl(0"0%"58%)                     ,
    /*247 */ "Grey62"//#9e9e9e	rgb(158"158"158)	hsl(0"0%"61%)                     ,
    /*248 */ "Grey66"//#a8a8a8	rgb(168"168"168)	hsl(0"0%"65%)                     ,
    /*249 */ "Grey70"//#b2b2b2	rgb(178"178"178)	hsl(0"0%"69%)                     ,
    /*250 */ "Grey74"//#bcbcbc	rgb(188"188"188)	hsl(0"0%"73%)                     ,
    /*251 */ "Grey78"//#c6c6c6	rgb(198"198"198)	hsl(0"0%"77%)                     ,
    /*252 */ "Grey82"//#d0d0d0	rgb(208"208"208)	hsl(0"0%"81%)                     ,
    /*253 */ "Grey85"//#dadada	rgb(218"218"218)	hsl(0"0%"85%)                     ,
    /*254 */ "Grey89"//#e4e4e4	rgb(228"228"228)	hsl(0"0%"89%)                     ,
    /*255 */ "Grey93"//#eeeeee	rgb(238"238"238)	hsl(0"0%"93%)                     ,
    /*256 */ "</span>"
        
};




std::string vxio::color::ansi(vxio::color::type C_, vxio::color::type BG_OR_OOB)
{
    if(C_ == Reset)
    {
        return "\033[0m";
    }
    iostr str = "\033[38;5;%dm";
    
    if(BG_OR_OOB != vxio::color::OOB)
        str += "\033[48;5;%dm";
    
    str << static_cast<int>(C_);
    
    if(BG_OR_OOB != vxio::color::OOB)
        str << static_cast<int>(BG_OR_OOB);
    
    return str();
}






// color_data
static std::vector<vxio::color_data> color_db = {
{vxio::color::Black,              0x00,0x00,0x00},//#000000	rgb(128,0,0)	hsl(0,100%,25%)
{vxio::color::Maroon,             0x80,0x00,0x00},//#800000	rgb(128,0,0)	hsl(0,100%,25%)
{vxio::color::Green,              0x00,0x80,0x00},
{vxio::color::Olive,              0x80,0x80,0x00},//#808000	rgb(128,128,0)	hsl(60,100%,25%)
{vxio::color::Navy,               0x00,0x00,0x80},//#000080	rgb(0,0,128)	hsl(240,100%,25%)
{vxio::color::Purple,             0x80,0x00,0x80},//#800080	rgb(128,0,128)	hsl(300,100%,25%)
{vxio::color::Teal,               0x00,0x80,0x80},//#008080	rgb(0,128,128)	hsl(180,100%,25%)
{vxio::color::Silver,             0xc0,0xc0,0xc0},//#c0c0c0	rgb(192,192,192)	hsl(0,0%,75%)
{vxio::color::Grey,               0x80,0x80,0x80},//#808080	rgb(128,128,128)	hsl(0,0%,50%)
{vxio::color::Red,                0xff,0x00,0x00},//#ff0000	rgb(255,0,0)	hsl(0,100%,50%)
{vxio::color::Lime,               0x00,0xff,0x00},//#00ff00	rgb(0,255,0)	hsl(120,100%,50%)
{vxio::color::Yellow,             0xff,0xff,0x00},//#ffff00	rgb(255,255,0)	hsl(60,100%,50%)
{vxio::color::Blue,               0x00,0x00,0xff},//#0000ff	rgb(0,0,255)	hsl(240,100%,50%)
{vxio::color::Fuchsia,            0xff,0x00,0xff},//#ff00ff	rgb(255,0,255)	hsl(300,100%,50%)
{vxio::color::Aqua,               0x00,0xff,0xff},//#00ffff	rgb(0,255,255)	hsl(180,100%,50%)
{vxio::color::White,              0xff,0xff,0xff},//#ffffff	rgb(255,255,255)	hsl(0,0%,100%)
{vxio::color::Grey0,              0x00,0x00,0x00},//#000000	rgb(0,0,0)	hsl(0,0%,0%)
{vxio::color::NavyBlue,           0x00,0x00,0x5f},//#00005f	rgb(0,0,95)	hsl(240,100%,18%)
{vxio::color::DarkBlue,           0x00,0x00,0x87},//#000087	rgb(0,0,135)	hsl(240,100%,26%)
{vxio::color::Blue3,              0x00,0x00,0xaf},//#0000af	rgb(0,0,175)	hsl(240,100%,34%)
{vxio::color::Blue4,              0x00,0x00,0xd7},//#0000d7	rgb(0,0,215)	hsl(240,100%,42%)
{vxio::color::Blue1,              0x00,0x00,0xff},//#0000ff	rgb(0,0,255)	hsl(240,100%,50%)
{vxio::color::DarkGreen,          0x00,0x5f,0x00},//#005f00	rgb(0,95,0)	hsl(120,100%,18%)
{vxio::color::DeepSkyBlue4,       0x00,0x5f,0x5f},//#005f5f	rgb(0,95,95)	hsl(180,100%,18%)
{vxio::color::DeepSkyBlue5,       0x00,0x5f,0x87},//#005f87	rgb(0,95,135)	hsl(97,100%,26%)
{vxio::color::DeepSkyBlue6,       0x00,0x5f,0xaf},//#005faf	rgb(0,95,175)	hsl(07,100%,34%)
{vxio::color::DodgerBlue3,        0x00,0x5f,0xd7},//#005fd7	rgb(0,95,215)	hsl(13,100%,42%)
{vxio::color::DodgerBlue2,        0x00,0x5f,0xff},//#005fff	rgb(0,95,255)	hsl(17,100%,50%)
{vxio::color::Green4,             0x00,0x87,0x00},//#008700	rgb(0,135,0)	hsl(120,100%,26%)
{vxio::color::SpringGreen4,       0x00,0x87,0x5f},//#00875f	rgb(0,135,95)	hsl(62,100%,26%)
{vxio::color::Turquoise4,         0x00,0x87,0x87},//#008787	rgb(0,135,135)	hsl(180,100%,26%)
{vxio::color::DeepSkyBlue3,       0x00,0x87,0xaf},//#0087af	rgb(0,135,175)	hsl(93,100%,34%)
{vxio::color::DeepSkyBlue7,       0x00,0x87,0xd7},//#0087d7	rgb(0,135,215)	hsl(02,100%,42%)
{vxio::color::DodgerBlue1,        0x00,0x87,0xff},//#0087ff	rgb(0,135,255)	hsl(08,100%,50%)
{vxio::color::Green3,             0x00,0xaf,0x00},//#00af00	rgb(0,175,0)	hsl(120,100%,34%)
{vxio::color::SpringGreen3,       0x00,0xaf,0x5f},//#00af5f	rgb(0,175,95)	hsl(52,100%,34%)
{vxio::color::DarkCyan,           0x00,0xaf,0x87},//#00af87	rgb(0,175,135)	hsl(66,100%,34%)
{vxio::color::LightSeaGreen,      0x00,0xaf,0xaf},//#00afaf	rgb(0,175,175)	hsl(180,100%,34%)
{vxio::color::DeepSkyBlue2,       0x00,0xaf,0xd7},//#00afd7	rgb(0,175,215)	hsl(91,100%,42%)
{vxio::color::DeepSkyBlue1,       0x00,0xaf,0xff},//#00afff	rgb(0,175,255)	hsl(98,100%,50%)
{vxio::color::Green5,             0x00,0xd7,0x00},//#00d700	rgb(0,215,0)	hsl(120,100%,42%)
{vxio::color::SpringGreen5,       0x00,0xd7,0x5f},//#00d75f	rgb(0,215,95)	hsl(46,100%,42%)
{vxio::color::SpringGreen2,       0x00,0xd7,0x87},//#00d787	rgb(0,215,135)	hsl(57,100%,42%)
{vxio::color::Cyan3,              0x00,0xd7,0xaf},//#00d7af	rgb(0,215,175)	hsl(68,100%,42%)
{vxio::color::DarkTurquoise,      0x00,0xd7,0xd7},//#00d7d7	rgb(0,215,215)	hsl(180,100%,42%)
{vxio::color::Turquoise2,         0x00,0xd7,0xff},//#00d7ff	rgb(0,215,255)	hsl(89,100%,50%)
{vxio::color::Green1,             0x00,0xff,0x00},//#00ff00	rgb(0,255,0)	hsl(120,100%,50%)
{vxio::color::SpringGreen6,       0x00,0xff,0x5f},//#00ff5f	rgb(0,255,95)	hsl(42,100%,50%)
{vxio::color::SpringGreen1,       0x00,0xff,0x87},//#00ff87	rgb(0,255,135)	hsl(51,100%,50%)
{vxio::color::MediumSpringGreen,  0x00,0xff,0xff},//#00ffaf	rgb(0,255,175)	hsl(61,100%,50%)
{vxio::color::Cyan2,              0x00,0xff,0xd7},//#00ffd7	rgb(0,255,215)	hsl(70,100%,50%)
{vxio::color::Cyan1,              0x00,0xff,0xff},//#00ffff	rgb(0,255,255)	hsl(180,100%,50%)
{vxio::color::DarkRed,            0x5f,0x00,0x00},//#5f0000	rgb(95,0,0)	hsl(0,100%,18%)
{vxio::color::DeepPink4,          0x5f,0x00,0x5f},//#5f005f	rgb(95,0,95)	hsl(300,100%,18%)
{vxio::color::Purple4,            0x5f,0x00,0x87},//#5f0087	rgb(95,0,135)	hsl(82,100%,26%)
{vxio::color::Purple5,            0x5f,0x00,0xaf},//#5f00af	rgb(95,0,175)	hsl(72,100%,34%)
{vxio::color::Purple3,            0x5f,0x00,0xd7},//#5f00d7	rgb(95,0,215)	hsl(66,100%,42%)
{vxio::color::BlueViolet,         0x5f,0x00,0xff},//#5f00ff	rgb(95,0,255)	hsl(62,100%,50%)
{vxio::color::Orange4,            0x5f,0x5f,0x00},//#5f5f00	rgb(95,95,0)	hsl(60,100%,18%)
{vxio::color::Grey37,             0x5f,0x5f,0x5f},//#5f5f5f	rgb(95,95,95)	hsl(0,0%,37%)
{vxio::color::MediumPurple4,      0x5f,0x5f,0x87},//#5f5f87	rgb(95,95,135)	hsl(240,17%,45%)
{vxio::color::SlateBlue3,         0x5f,0x5f,0xaf},//#5f5faf	rgb(95,95,175)	hsl(240,33%,52%)
{vxio::color::SlateBlue4,         0x5f,0x5f,0xd7},//#5f5fd7	rgb(95,95,215)	hsl(240,60%,60%)
{vxio::color::RoyalBlue1,         0x5f,0x5f,0xff},//#5f5fff	rgb(95,95,255)	hsl(240,100%,68%)
{vxio::color::Chartreuse4,        0x5f,0x87,0x00},//#5f8700	rgb(95,135,0)	hsl(7,100%,26%)
{vxio::color::DarkSeaGreen4,      0x5f,0x87,0x5f},//#5f875f	rgb(95,135,95)	hsl(120,17%,45%)
{vxio::color::PaleTurquoise4,     0x5f,0x87,0x87},//#5f8787	rgb(95,135,135)	hsl(180,17%,45%)
{vxio::color::SteelBlue,          0x5f,0x87,0xaf},//#5f87af	rgb(95,135,175)	hsl(210,33%,52%)
{vxio::color::SteelBlue3,         0x5f,0x87,0xd7},//#5f87d7	rgb(95,135,215)	hsl(220,60%,60%)
{vxio::color::CornflowerBlue,     0x5f,0x87,0xff},//#5f87ff	rgb(95,135,255)	hsl(225,100%,68%)
{vxio::color::Chartreuse3,        0x5f,0xaf,0x00},//#5faf00	rgb(95,175,0)	hsl(7,100%,34%)
{vxio::color::DarkSeaGreen5,      0x5f,0xaf,0x5f},//#5faf5f	rgb(95,175,95)	hsl(120,33%,52%)
{vxio::color::CadetBlue,          0x5f,0xaf,0x87},//#5faf87	rgb(95,175,135)	hsl(150,33%,52%)
{vxio::color::CadetBlue2,         0x5f,0xaf,0xaf},//#5fafaf	rgb(95,175,175)	hsl(180,33%,52%)
{vxio::color::SkyBlue3,           0x5f,0xaf,0xd7},//#5fafd7	rgb(95,175,215)	hsl(200,60%,60%)
{vxio::color::SteelBlue1,         0x5f,0xaf,0xff},//#5fafff	rgb(95,175,255)	hsl(210,100%,68%)
{vxio::color::Chartreuse5,        0x5f,0xd7,0x00},//#5fd700	rgb(95,215,0)	hsl(3,100%,42%)
{vxio::color::PaleGreen3,         0x5f,0xd7,0x5f},//#5fd75f	rgb(95,215,95)	hsl(120,60%,60%)
{vxio::color::SeaGreen3,          0x5f,0xd7,0x87},//#5fd787	rgb(95,215,135)	hsl(140,60%,60%)
{vxio::color::Aquamarine3,        0x5f,0xd7,0xaf},//#5fd7af	rgb(95,215,175)	hsl(160,60%,60%)
{vxio::color::MediumTurquoise,    0x5f,0xd7,0xd7},//#5fd7d7	rgb(95,215,215)	hsl(180,60%,60%)
{vxio::color::SteelBlue4,         0x5f,0xd7,0xff},//#5fd7ff	rgb(95,215,255)	hsl(195,100%,68%)
{vxio::color::Chartreuse2,        0x5f,0xff,0x00},//#5fff00	rgb(95,255,0)	hsl(7,100%,50%)
{vxio::color::SeaGreen2,          0x5f,0xff,0x5f},//#5fff5f	rgb(95,255,95)	hsl(120,100%,68%)
{vxio::color::SeaGreen1,          0x5f,0xff,0x87},//#5fff87	rgb(95,255,135)	hsl(135,100%,68%)
{vxio::color::SeaGreen4,          0x5f,0xff,0xaf},//#5fffaf	rgb(95,255,175)	hsl(150,100%,68%)
{vxio::color::Aquamarine1,        0x5f,0xff,0xd7},//#5fffd7	rgb(95,255,215)	hsl(165,100%,68%)
{vxio::color::DarkSlateGray2,     0x5f,0xff,0xff},//#5fffff	rgb(95,255,255)	hsl(180,100%,68%)
{vxio::color::DarkRed2,           0x87,0x00,0x00},//#870000	rgb(135,0,0)	hsl(0,100%,26%)
{vxio::color::DeepPink5,          0x87,0x00,0x5f},//#87005f	rgb(135,0,95)	hsl(17,100%,26%)
{vxio::color::DarkMagenta,        0x87,0x00,0x87},//#870087	rgb(135,0,135)	hsl(300,100%,26%)
{vxio::color::DarkMagenta2,       0x87,0x00,0xaf},//#8700af	rgb(135,0,175)	hsl(86,100%,34%)
{vxio::color::DarkViolet,         0x87,0x00,0xd7},//#8700d7	rgb(135,0,215)	hsl(77,100%,42%)
{vxio::color::Purple2,            0x87,0x00,0xff},//#8700ff	rgb(135,0,255)	hsl(71,100%,50%)
{vxio::color::Orange5,            0x87,0x5f,0x00},//#875f00	rgb(135,95,0)	hsl(2,100%,26%)
{vxio::color::LightPink4,         0x87,0x5f,0x5f},//#875f5f	rgb(135,95,95)	hsl(0,17%,45%)
{vxio::color::Plum4,              0x87,0x5f,0x87},//#875f87	rgb(135,95,135)	hsl(300,17%,45%)
{vxio::color::MediumPurple3,      0x87,0x5f,0xaf},//#875faf	rgb(135,95,175)	hsl(270,33%,52%)
{vxio::color::MediumPurple5,      0x87,0x5f,0xd7},//#875fd7	rgb(135,95,215)	hsl(260,60%,60%)
{vxio::color::SlateBlue1,         0x87,0x5f,0xff},//#875fff	rgb(135,95,255)	hsl(255,100%,68%)
{vxio::color::Yellow4,            0x87,0x87,0x00},//#878700	rgb(135,135,0)	hsl(60,100%,26%)
{vxio::color::Wheat4,             0x87,0x87,0x5f},//#87875f	rgb(135,135,95)	hsl(60,17%,45%)
{vxio::color::Grey53,             0x87,0x87,0x87},//#878787	rgb(135,135,135)	hsl(0,0%,52%)
{vxio::color::LightSlateGrey,     0x87,0x87,0xaf},//#8787af	rgb(135,135,175)	hsl(240,20%,60%)
{vxio::color::MediumPurple,       0x87,0x87,0xd7},//#8787d7	rgb(135,135,215)	hsl(240,50%,68%)
{vxio::color::LightSlateBlue,     0x87,0x87,0xff},//#8787ff	rgb(135,135,255)	hsl(240,100%,76%)
{vxio::color::Yellow5,            0x87,0xaf,0x00},//#87af00	rgb(135,175,0)	hsl(3,100%,34%)
{vxio::color::DarkOliveGreen3,    0x87,0xaf,0x5f},//#87af5f	rgb(135,175,95)	hsl(90,33%,52%)
{vxio::color::DarkSeaGreen,       0x87,0xaf,0x87},//#87af87	rgb(135,175,135)	hsl(120,20%,60%)
{vxio::color::LightSkyBlue3,      0x87,0xaf,0xaf},//#87afaf	rgb(135,175,175)	hsl(180,20%,60%)
{vxio::color::LightSkyBlue4,      0x87,0xaf,0xd7},//#87afd7	rgb(135,175,215)	hsl(210,50%,68%)
{vxio::color::SkyBlue2,           0x87,0xaf,0xff},//#87afff	rgb(135,175,255)	hsl(220,100%,76%)
{vxio::color::Chartreuse6,        0x87,0xd7,0x00},//#87d700	rgb(135,215,0)	hsl(2,100%,42%)
{vxio::color::DarkOliveGreen4,    0x87,0xd7,0x5f},//#87d75f	rgb(135,215,95)	hsl(100,60%,60%)
{vxio::color::PaleGreen4,         0x87,0xd7,0x87},//#87d787	rgb(135,215,135)	hsl(120,50%,68%)
{vxio::color::DarkSeaGreen3,      0x87,0xd7,0xaf},//#87d7af	rgb(135,215,175)	hsl(150,50%,68%)
{vxio::color::DarkSlateGray3,     0x87,0xd7,0xd7},//#87d7d7	rgb(135,215,215)	hsl(180,50%,68%)
{vxio::color::SkyBlue1,           0x87,0xd7,0xff},//#0x87,0xd7,0xff	rgb(135,215,255)	hsl(200,100%,76%)
{vxio::color::Chartreuse1,        0x87,0xff,0x00},//#87ff00	rgb(135,255,0)	hsl(8,100%,50%)
{vxio::color::LightGreen,         0x87,0xff,0x5f},//#87ff5f	rgb(135,255,95)	hsl(105,100%,68%)
{vxio::color::LightGreen2,        0x87,0xff,0x87},//#87ff87	rgb(135,255,135)	hsl(120,100%,76%)
{vxio::color::PaleGreen1,         0x87,0xff,0xaf},//#87ffaf	rgb(135,255,175)	hsl(140,100%,76%)
{vxio::color::Aquamarine2,        0x87,0xff,0xd7},//#87ffd7	rgb(135,255,215)	hsl(160,100%,76%)
{vxio::color::DarkSlateGray1,     0x87,0xff,0xff},//#87ffff	rgb(135,255,255)	hsl(180,100%,76%)
{vxio::color::Red3,               0xaf,0x00,0x00},//#af0000	rgb(175,0,0)	hsl(0,100%,34%)
{vxio::color::DeepPink6,          0xaf,0x00,0x5f},//#af005f	rgb(175,0,95)	hsl(27,100%,34%)
{vxio::color::MediumVioletRed,    0xaf,0x00,0x87},//#af0087	rgb(175,0,135)	hsl(13,100%,34%)
{vxio::color::Magenta3,           0xaf,0x00,0xaf},//#af00af	rgb(175,0,175)	hsl(300,100%,34%)
{vxio::color::DarkViole2,         0xaf,0x00,0xd7},//#af00d7	rgb(175,0,215)	hsl(88,100%,42%)
{vxio::color::Purple6,            0xaf,0x00,0xff},//#af00ff	rgb(175,0,255)	hsl(81,100%,50%)
{vxio::color::DarkOrange3,        0xaf,0x5f,0x00},//#af5f00	rgb(175,95,0)	hsl(2,100%,34%)
{vxio::color::IndianRed,          0xaf,0x5f,0x5f},//#af5f5f	rgb(175,95,95)	hsl(0,33%,52%)
{vxio::color::HotPink3,           0xaf,0x5f,0x87},//#af5f87	rgb(175,95,135)	hsl(330,33%,52%)
{vxio::color::MediumOrchid3,      0xaf,0x5f,0xaf},//#af5faf	rgb(175,95,175)	hsl(300,33%,52%)
{vxio::color::MediumOrchid,       0xaf,0x5f,0xd7},//#af5fd7	rgb(175,95,215)	hsl(280,60%,60%)
{vxio::color::MediumPurple2,      0xaf,0x5f,0xff},//#af5fff	rgb(175,95,255)	hsl(270,100%,68%)
{vxio::color::DarkGoldenrod,      0xaf,0x87,0x00},//#af8700	rgb(175,135,0)	hsl(6,100%,34%)
{vxio::color::LightSalmon3,       0xaf,0x87,0x5f},//#af875f	rgb(175,135,95)	hsl(30,33%,52%)
{vxio::color::RosyBrown,          0xaf,0x87,0x87},//#af8787	rgb(175,135,135)	hsl(0,20%,60%)
{vxio::color::Grey63,             0xaf,0x87,0xaf},//#af87af	rgb(175,135,175)	hsl(300,20%,60%)
{vxio::color::MediumPurple6,      0xaf,0x87,0xd7},//#af87d7	rgb(175,135,215)	hsl(270,50%,68%)
{vxio::color::MediumPurple1,      0xaf,0x87,0xff},//#af87ff	rgb(175,135,255)	hsl(260,100%,76%)
{vxio::color::Gold3,              0xaf,0xaf,0x00},//#afaf00	rgb(175,175,0)	hsl(60,100%,34%)
{vxio::color::DarkKhaki,          0xaf,0xaf,0x5f},//#afaf5f	rgb(175,175,95)	hsl(60,33%,52%)
{vxio::color::NavajoWhite3,       0xaf,0xaf,0x87},//#afaf87	rgb(175,175,135)	hsl(60,20%,60%)
{vxio::color::Grey69,             0xaf,0xaf,0xaf},//#afafaf	rgb(175,175,175)	hsl(0,0%,68%)
{vxio::color::LightSteelBlue3,    0xaf,0xaf,0xd7},//#afafd7	rgb(175,175,215)	hsl(240,33%,76%)
{vxio::color::LightSteelBlue,     0xaf,0xaf,0xff},//#afafff	rgb(175,175,255)	hsl(240,100%,84%)
{vxio::color::Yellow3,            0xaf,0xd7,0x00},//#afd700	rgb(175,215,0)	hsl(1,100%,42%)
{vxio::color::DarkOliveGreen5,    0xaf,0xd7,0x5f},//#afd75f	rgb(175,215,95)	hsl(80,60%,60%)
{vxio::color::DarkSeaGreen6,      0xaf,0xd7,0x87},//#afd787	rgb(175,215,135)	hsl(90,50%,68%)
{vxio::color::DarkSeaGreen2,      0xaf,0xd7,0xaf},//#afd7af	rgb(175,215,175)	hsl(120,33%,76%)
{vxio::color::LightCyan3,         0xaf,0xd7,0xd7},//#afd7d7	rgb(175,215,215)	hsl(180,33%,76%)
{vxio::color::LightSkyBlue1,      0xaf,0xd7,0xff},//#afd7ff	rgb(175,215,255)	hsl(210,100%,84%)
{vxio::color::GreenYellow,        0xaf,0xff,0x00},//#afff00	rgb(175,255,0)	hsl(8,100%,50%)
{vxio::color::DarkOliveGreen2,    0xaf,0xff,0x5f},//#afff5f	rgb(175,255,95)	hsl(90,100%,68%)
{vxio::color::PaleGreen2,         0xaf,0xff,0x87},//#afff87	rgb(175,255,135)	hsl(100,100%,76%)
{vxio::color::DarkSeaGreen7,      0xaf,0xff,0xaf},//#afffaf	rgb(175,255,175)	hsl(120,100%,84%)
{vxio::color::DarkSeaGreen1,      0xaf,0xff,0xd7},//#afffd7	rgb(175,255,215)	hsl(150,100%,84%)
{vxio::color::PaleTurquoise1,     0xaf,0xff,0xff},//#afffff	rgb(175,255,255)	hsl(180,100%,84%)
{vxio::color::Red4,               0xd7,0x00,0x00},//#d70000	rgb(215,0,0)	hsl(0,100%,42%)
{vxio::color::DeepPink3,          0xd7,0x00,0x5f},//#d7005f	rgb(215,0,95)	hsl(33,100%,42%)
{vxio::color::DeepPink7,          0xd7,0x00,0x87},//#d70087	rgb(215,0,135)	hsl(22,100%,42%)
{vxio::color::Magenta4,           0xd7,0x00,0xaf},//#d700af	rgb(215,0,175)	hsl(11,100%,42%)
{vxio::color::Magenta5,           0xd7,0x00,0xd7},//#d700d7	rgb(215,0,215)	hsl(300,100%,42%)
{vxio::color::Magenta2,           0xd7,0x00,0xff},//#d700ff	rgb(215,0,255)	hsl(90,100%,50%)
{vxio::color::DarkOrange4,        0xd7,0x5f,0x00},//#d75f00	rgb(215,95,0)	hsl(6,100%,42%)
{vxio::color::IndianRed2,         0xd7,0x5f,0x5f},//#d75f5f	rgb(215,95,95)	hsl(0,60%,60%)
{vxio::color::HotPink4,           0xd7,0x5f,0x87},//#d75f87	rgb(215,95,135)	hsl(340,60%,60%)
{vxio::color::HotPink2,           0xd7,0x5f,0xaf},//#d75faf	rgb(215,95,175)	hsl(320,60%,60%)
{vxio::color::Orchid,             0xd7,0x5f,0xd7},//#d75fd7	rgb(215,95,215)	hsl(300,60%,60%)
{vxio::color::MediumOrchid1,      0xd7,0x5f,0xff},//#d75fff	rgb(215,95,255)	hsl(285,100%,68%)
{vxio::color::Orange3,            0xd7,0x87,0x00},//#d78700	rgb(215,135,0)	hsl(7,100%,42%)
{vxio::color::LightSalmon4,       0xd7,0x87,0x5f},//#d7875f	rgb(215,135,95)	hsl(20,60%,60%)
{vxio::color::LightPink3,         0xd7,0x87,0x87},//#d78787	rgb(215,135,135)	hsl(0,50%,68%)
{vxio::color::Pink3,              0xd7,0x87,0xaf},//#d787af	rgb(215,135,175)	hsl(330,50%,68%)
{vxio::color::Plum3,              0xd7,0x87,0xd7},//#d787d7	rgb(215,135,215)	hsl(300,50%,68%)
{vxio::color::Violet,             0xd7,0x87,0xff},//#d787ff	rgb(215,135,255)	hsl(280,100%,76%)
{vxio::color::Gold4,              0xd7,0xaf,0x00},//#d7af00	rgb(215,175,0)	hsl(8,100%,42%)
{vxio::color::LightGoldenrod3,    0xd7,0xaf,0x5f},//#d7af5f	rgb(215,175,95)	hsl(40,60%,60%)
{vxio::color::Tan,                0xd7,0xaf,0x87},//#d7af87	rgb(215,175,135)	hsl(30,50%,68%)
{vxio::color::MistyRose3,         0xd7,0xaf,0xaf},//#d7afaf	rgb(215,175,175)	hsl(0,33%,76%)
{vxio::color::Thistle3,           0xd7,0xaf,0xd7},//#d7afd7	rgb(215,175,215)	hsl(300,33%,76%)
{vxio::color::Plum2,              0xd7,0xaf,0xff},//#d7afff	rgb(215,175,255)	hsl(270,100%,84%)
{vxio::color::Yellow6,            0xd7,0xd7,0x00},//#d7d700	rgb(215,215,0)	hsl(60,100%,42%)
{vxio::color::Khaki3,             0xd7,0xd7,0x5f},//#d7d75f	rgb(215,215,95)	hsl(60,60%,60%)
{vxio::color::LightGoldenrod2,    0xd7,0xd7,0x87},//#d7d787	rgb(215,215,135)	hsl(60,50%,68%)
{vxio::color::LightYellow3,       0xd7,0xd7,0xaf},//#d7d7af	rgb(215,215,175)	hsl(60,33%,76%)
{vxio::color::Grey84,             0xd7,0xd7,0xd7},//#d7d7d7	rgb(215,215,215)	hsl(0,0%,84%)
{vxio::color::LightSteelBlue1,    0xd7,0xd7,0xff},//#d7d7ff	rgb(215,215,255)	hsl(240,100%,92%)
{vxio::color::Yellow2,            0xd7,0xff,0x00},//#d7ff00	rgb(215,255,0)	hsl(9,100%,50%)
{vxio::color::DarkOliveGreen1,    0xd7,0xff,0x5f},//#d7ff5f	rgb(215,255,95)	hsl(75,100%,68%)
{vxio::color::DarkOliveGreen6,    0xd7,0xff,0x87},//#d7ff87	rgb(215,255,135)	hsl(80,100%,76%)
{vxio::color::DarkSeaGreen8,      0xd7,0xff,0xaf},//#d7ffaf	rgb(215,255,175)	hsl(90,100%,84%)
{vxio::color::Honeydew2,          0xd7,0xff,0xd7},//#d7ffd7	rgb(215,255,215)	hsl(120,100%,92%)
{vxio::color::LightCyan1,         0xd7,0xff,0xff},//#d7ffff	rgb(215,255,255)	hsl(180,100%,92%)
{vxio::color::Red1,               0xff,0x00,0x00},//#ff0000	rgb(255,0,0)	hsl(0,100%,50%)
{vxio::color::DeepPink2,          0xff,0x00,0x5f},//#ff005f	rgb(255,0,95)	hsl(37,100%,50%)
{vxio::color::DeepPink1,          0xff,0x00,0x87},//#ff0087	rgb(255,0,135)	hsl(28,100%,50%)
{vxio::color::DeepPink8,          0xff,0x00,0xaf},//#ff00af	rgb(255,0,175)	hsl(18,100%,50%)
{vxio::color::Magenta6,           0xff,0x00,0xd7},//#ff00d7	rgb(255,0,215)	hsl(09,100%,50%)
{vxio::color::Magenta1,           0xff,0x00,0xff},//#ff00ff	rgb(255,0,255)	hsl(300,100%,50%)
{vxio::color::OrangeRed1,         0xff,0x5f,0x00},//#ff5f00	rgb(255,95,0)	hsl(2,100%,50%)
{vxio::color::IndianRed1,         0xff,0x5f,0x5f},//#ff5f5f	rgb(255,95,95)	hsl(0,100%,68%)
{vxio::color::IndianRed3,         0xff,0x5f,0x87},//#ff5f87	rgb(255,95,135)	hsl(345,100%,68%)
{vxio::color::HotPink,            0xff,0x5f,0xaf},//#ff5faf	rgb(255,95,175)	hsl(330,100%,68%)
{vxio::color::HotPin2,            0xff,0x5f,0xd7},//#ff5fd7	rgb(255,95,215)	hsl(315,100%,68%)
{vxio::color::MediumOrchid2,      0xff,0x5f,0xff},//#ff5fff	rgb(255,95,255)	hsl(300,100%,68%)
{vxio::color::DarkOrange,         0xff,0x87,0x00},//#ff8700	rgb(255,135,0)	hsl(1,100%,50%)
{vxio::color::Salmon1,            0xff,0x87,0x5f},//#ff875f	rgb(255,135,95)	hsl(15,100%,68%)
{vxio::color::LightCoral,         0xff,0x87,0x87},//#ff8787	rgb(255,135,135)	hsl(0,100%,76%)
{vxio::color::PaleVioletRed1,     0xff,0x87,0xaf},//#ff87af	rgb(255,135,175)	hsl(340,100%,76%)
{vxio::color::Orchid2,            0xff,0x87,0xd7},//#ff87d7	rgb(255,135,215)	hsl(320,100%,76%)
{vxio::color::Orchid1,            0xff,0x87,0xff},//#ff87ff	rgb(255,135,255)	hsl(300,100%,76%)
{vxio::color::Orange1,            0xff,0xaf,0x00},//#ffaf00	rgb(255,175,0)	hsl(1,100%,50%)
{vxio::color::SandyBrown,         0xff,0xaf,0x5f},//#ffaf5f	rgb(255,175,95)	hsl(30,100%,68%)
{vxio::color::LightSalmon1,       0xff,0xaf,0x87},//#ffaf87	rgb(255,175,135)	hsl(20,100%,76%)
{vxio::color::LightPink1,         0xff,0xaf,0xaf},//#ffafaf	rgb(255,175,175)	hsl(0,100%,84%)
{vxio::color::Pink1,              0xff,0xaf,0xd7},//#ffafd7	rgb(255,175,215)	hsl(330,100%,84%)
{vxio::color::Plum1,              0xff,0xaf,0xff},//#ffafff	rgb(255,175,255)	hsl(300,100%,84%)
{vxio::color::Gold1,              0xff,0xd7,0x00},//#ffd700	rgb(255,215,0)	hsl(0,100%,50%)
{vxio::color::LightGoldenrod4,    0xff,0xd7,0x5f},//#ffd75f	rgb(255,215,95)	hsl(45,100%,68%)
{vxio::color::LightGoldenrod5,    0xff,0xd7,0x87},//#ffd787	rgb(255,215,135)	hsl(40,100%,76%)
{vxio::color::NavajoWhite1,       0xff,0xd7,0xaf},//#ffd7af	rgb(255,215,175)	hsl(30,100%,84%)
{vxio::color::MistyRose1,         0xff,0xd7,0xd7},//#ffd7d7	rgb(255,215,215)	hsl(0,100%,92%)
{vxio::color::Thistle1,           0xff,0xd7,0xff},//#ffd7ff	rgb(255,215,255)	hsl(300,100%,92%)
{vxio::color::Yellow1,            0xff,0xff,0x00},//#ffff00	rgb(255,255,0)	hsl(60,100%,50%)
{vxio::color::LightGoldenrod1,    0xff,0xff,0x5f},//#ffff5f	rgb(255,255,95)	hsl(60,100%,68%)
{vxio::color::Khaki1,             0xff,0xff,0x87},//#ffff87	rgb(255,255,135)	hsl(60,100%,76%)
{vxio::color::Wheat1,             0xff,0xff,0xaf},//#ffffaf	rgb(255,255,175)	hsl(60,100%,84%)
{vxio::color::Cornsilk1,          0xff,0xff,0xd7},//#ffffd7	rgb(255,255,215)	hsl(60,100%,92%)
{vxio::color::Grey100,            0xff,0xff,0xff},//#ffffff	rgb(255,255,255)	hsl(0,0%,100%)
{vxio::color::Grey3,              0x08,0x08,0x08},//#080808	rgb(8,8,8)	hsl(0,0%,3%)
{vxio::color::Grey7,              0x12,0x12,0x12},//#121212	rgb(18,18,18)	hsl(0,0%,7%)
{vxio::color::Grey11,             0x1c,0x1c,0x1c},//#1c1c1c	rgb(28,28,28)	hsl(0,0%,10%)
{vxio::color::Grey15,             0x26,0x26,0x26},//#262626	rgb(38,38,38)
{vxio::color::Grey19,             0x30,0x30,0x30},//#303030	rgb(48,48,48)	hsl(0,0%,18%)
{vxio::color::Grey23,             0x3a,0x3a,0x3a},//#3a3a3a	rgb(58,58,58)	hsl(0,0%,22%)
{vxio::color::Grey27,             0x44,0x44,0x44},//#444444	rgb(68,68,68)	hsl(0,0%,26%)
{vxio::color::Grey30,             0x4e,0x4e,0x4e},//#4e4e4e	rgb(78,78,78)	hsl(0,0%,30%)
{vxio::color::Grey35,             0x58,0x58,0x58},//#585858	rgb(88,88,88)	hsl(0,0%,34%)
{vxio::color::Grey39,             0x62,0x62,0x62},//#626262	rgb(98,98,98)	hsl(0,0%,37%)
{vxio::color::Grey42,             0x6c,0x6c,0x6c},//#6c6c6c	rgb(108,108,108)	hsl(0,0%,40%)
{vxio::color::Grey46,             0x76,0x76,0x76},//#767676	rgb(118,118,118)	hsl(0,0%,46%)
{vxio::color::Grey50,             0x80,0x80,0x80},//#808080	rgb(128,128,128)	hsl(0,0%,50%)
{vxio::color::Grey54,             0x8a,0x8a,0x8a},//#8a8a8a	rgb(138,138,138)	hsl(0,0%,54%)
{vxio::color::Grey58,             0x94,0x94,0x94},//#949494	rgb(148,148,148)	hsl(0,0%,58%)
{vxio::color::Grey62,             0x9e,0x9e,0x9e},//#9e9e9e	rgb(158,158,158)	hsl(0,0%,61%)
{vxio::color::Grey66,             0xa8,0xa8,0xa8},//#a8a8a8	rgb(168,168,168)	hsl(0,0%,65%)
{vxio::color::Grey70,             0xb2,0xb2,0xb2},//#b2b2b2	rgb(178,178,178)	hsl(0,0%,69%)
{vxio::color::Grey74,             0xbc,0xbc,0xbc},//#bcbcbc	rgb(188,188,188)	hsl(0,0%,73%)
{vxio::color::Grey78,             0xc6,0xc6,0xc6},//#c6c6c6	rgb(198,198,198)	hsl(0,0%,77%)
{vxio::color::Grey82,             0xd0,0xd0,0xd0},//#d0d0d0	rgb(208,208,208)	hsl(0,0%,81%)
{vxio::color::Grey85,             0xda,0xda,0xda},//#dadada	rgb(218,218,218)	hsl(0,0%,85%)
{vxio::color::Grey89,             0xe4,0xe4,0xe4},//#e4e4e4	rgb(228,228,228)	hsl(0,0%,89%)
{vxio::color::Grey93,             0xee,0xee,0xee},//#eeeeee	rgb(238,238,238)	hsl(0,0%,93%)
{vxio::color::Reset,0,0,0},
{vxio::color::OOB,0,0,0}
};


/*!
 * @brief Produces the html element for displaying the subsequent text with the given color
 * @param color_name mnemonic
 * @return std::string containing the place-holder to be filled subsequently with the text.
 */
std::string vxio::color::html(vxio::color::type color_name)
{
    iostr str = "<span style=\"color:%s;\">%s</span>";
    str << color_db[color_name].to_string();
    return str;
}

// uint32_t vxio::color_data::rgb() const
// {
//     return (*(uint32_t*)this);
// }

/**
 * @brief
 *
 * @author &copy;2022 Serge Lussier (lussier.serge@gmail.com); oldlonecoder
 * @return std::string contents of the rgb in html format "#RRGGBB"
 *
 * @note
 */

std::string vxio::color_data::to_string() const
{
    iostr str = "#%02X%02X%02X";
    str << r << g << b;

    //iostr str = "#%02X%02X%02X\n%08b";
    //str << r << g << b << ( (r << 16) | (g << 8) | b);
    return str();
}


std::string vxio::color::hrgb(vxio::color::type color_index)
{
    iostr str = "#%02X%02X%02X";
    color_data cd = color_db[color_index];
    str << cd.r << cd.g << cd.b;

    //iostr str = "#%02X%02X%02X\n%08b";
    //str << r << g << b << ( (r << 16) | (g << 8) | b);
    return str();
}
