#include <vxio/lexer/token_data.h>
#include <map>


static const char *fmt = "'%s'(%d,%d) %s[%s]";

namespace vxio
{

static std::map<mnemonic, vxio::type::T> key_to_type = {{mnemonic::k_i8,    vxio::type::i8_t},
                                                          {mnemonic::k_i16,   vxio::type::i16_t},
                                                          {mnemonic::k_i32,   vxio::type::i32_t},
                                                          {mnemonic::k_i64,   vxio::type::i64_t},
                                                          {mnemonic::k_u8,    vxio::type::u8_t},
                                                          {mnemonic::k_u16,   vxio::type::u16_t},
                                                          {mnemonic::k_u32,   vxio::type::u32_t},
                                                          {mnemonic::k_u64,   vxio::type::u64_t},
                                                          {mnemonic::k_float, vxio::type::float_t}
    
};

std::string token_data::location_data::operator()() const
{
    // line y, col x: `attribute`
    return text();
}

std::string token_data::location_data::text() const
{
    if(begin == nullptr)
        return std::string();
    
    std::string str;
    const char *ptr = begin;
    //str += '\'';
    str += *ptr++;
    if(!end)
    {
        while(*ptr)
            str += *ptr++;
        return str;
    }
    while(ptr <= end)
        str += *ptr++;
    //str += '\'';
    return str;
}

std::string token_data::location_data::position() const
{
    iostr Str = "%d,%d";
    Str << linenum << colnum;
    return Str();
}

token_data VXIO_API_DLL token_data::mNull = token_data();

token_data::token_data(mnemonic aCode, vxio::type::T aType, vxio::type::T aSem, vxio::type::distance::T aDelta, lexem::T aLexem, uint8_t V_Flag)
{
    c      = aCode;
    t      = aType;
    s      = aSem;
    d      = aDelta;
    mLoc   = {aLexem, 0, 0, 0, -1};
    _flags = {V_Flag, 0, 0};
    
}

token_data::token_data(mnemonic aCode, vxio::type::T aType, vxio::type::T aSem, vxio::type::distance::T aDelta, token_data::location_data aLoc, token_data::Flag aFlag, void *aPtr)
{
    c      = aCode;
    t      = aType;
    s      = aSem;
    d      = aDelta;
    mLoc   = aLoc;
    _flags = aFlag;
    vdata  = aPtr;
}

token_data::token_data(const token_data &aToken) // Consider using "default" ...
{
    c      = aToken.c;
    t      = aToken.t;
    s      = aToken.s;
    d      = aToken.d;
    mLoc   = aToken.mLoc;
    _flags = aToken._flags;
    vdata  = aToken.vdata;
}

token_data::token_data(token_data &&aToken) noexcept
{
    std::swap(c, aToken.c);
    std::swap(t, aToken.t);
    std::swap(s, aToken.s);
    std::swap(d, aToken.d);
    std::swap(mLoc, aToken.mLoc);
    std::swap(_flags, aToken._flags);
    std::swap(vdata, aToken.vdata);
    
}

token_data &token_data::operator=(token_data &&aToken) noexcept
{
    std::swap(c, aToken.c);
    std::swap(t, aToken.t);
    std::swap(s, aToken.s);
    std::swap(d, aToken.d);
    std::swap(mLoc, aToken.mLoc);
    std::swap(_flags, aToken._flags);
    std::swap(vdata, aToken.vdata);
    return *this;
}

token_data &token_data::operator=(const token_data &aToken)
{
    c      = aToken.c;
    t      = aToken.t;
    s      = aToken.s;
    d      = aToken.d;
    mLoc   = aToken.mLoc;
    _flags = aToken._flags;
    vdata  = aToken.vdata;
    return *this;
}

std::string token_data::mark() const
{
    iostr str;
    const char *B = mLoc.begin - mLoc.offset;
    //std::string str;
    int l = 1;
    const char *cc = mLoc.begin;
    // localiser le debut de la ligne;
    while(*cc && (cc > B) && (*cc != '\n') && (*cc != '\r'))
        --cc;
    // debut de la ligne ou de la source:
    if(cc >= B)
    {
        if((*cc == '\n') || (*cc == '\r'))
            ++cc;
        while(*cc && (*cc != '\n') && (*cc != '\r'))
            str += *cc++;
    }
    iostr tstr;
    
    tstr << str << '\n';
    for(int x = 1; x < mLoc.colnum; x++)
        tstr << ' ';
    tstr << '^';
    //std::cerr << tstr() << '\n';
    return tstr();
}

std::string token_data::semantic_types() const
{
    std::string Str;
    return vxio::type::name(s);
}

std::string token_data::type_name() const
{
    std::string Str;
    return vxio::type::name(t);
}

static token_data::collection tokens_table = {{mnemonic::k_null,               vxio::type::keyword_t,    0x000040000,  vxio::type::distance::identifier, lexem::k_null,        1},
                                              {mnemonic::k_left_shift,         vxio::type::binary_t,     0x000C00000,  vxio::type::distance::shift,      lexem::k_left_shift,  1},
                                              {mnemonic::k_unshadow,           vxio::type::prefix_t,     0x000580000,  vxio::type::distance::scope,      lexem::k_unshadow,    1},
                                              {mnemonic::k_open_abs_op,        vxio::type::open_pair_t,  0x080C00000,  vxio::type::distance::paranthese, lexem::k_abs_begin,   0},
                                              {mnemonic::k_close_abs_op,       vxio::type::close_pair_t, 0x080400000,  vxio::type::distance::paranthese, lexem::k_abs_end,     0},
                                              {mnemonic::k_radical,            vxio::type::binary_t,     0x000C00000,  vxio::type::distance::exponent,   lexem::k_radical,     1},
                                              {mnemonic::k_exponent,           vxio::type::binary_t,     0x000C00000,  vxio::type::distance::exponent,   lexem::k_exponent,    1},
                                              {mnemonic::k_right_shift,        vxio::type::binary_t,     0x000400000,  vxio::type::distance::shift,      lexem::k_right_shift, 1},
                                              {mnemonic::k_binary_xor,         vxio::type::binary_t,     0x000C00000,  vxio::type::distance::bits,       lexem::k_xor,         1},
                                              {mnemonic::k_decr,               vxio::type::unary_t,      0x000580000,  vxio::type::distance::unary,      lexem::k_decr,        1},
                                              {mnemonic::k_incr,               vxio::type::unary_t,      0x000580000,  vxio::type::distance::unary,      lexem::k_incr,        1},
                                              {mnemonic::k_assign_add,         vxio::type::assign_t,     0x100C40000,  vxio::type::distance::assign,     lexem::k_assign_add,  1},
                                              {mnemonic::k_assign_sub,         vxio::type::assign_t,     0x100C40000,  vxio::type::distance::assign,     lexem::k_assign_sub,  1},
                                              {mnemonic::k_assign_mul,         vxio::type::assign_t,     0x100C40000,  vxio::type::distance::assign,     lexem::k_assign_mul,  1},
                                              {mnemonic::k_assign_div,         vxio::type::assign_t,     0x100C40000,  vxio::type::distance::assign,     lexem::k_assign_div,  1},
                                              {mnemonic::k_assign_mod,         vxio::type::assign_t,     0x100C40000,  vxio::type::distance::assign,     lexem::k_assign_mod,  1},
                                              {mnemonic::k_assign_and,         vxio::type::assign_t,     0x100C40000,  vxio::type::distance::assign,     lexem::k_assign_and,  1},
                                              {mnemonic::k_assign_or,          vxio::type::assign_t,     0x100C40000,  vxio::type::distance::assign,     lexem::k_assign_or,   1},
                                              {mnemonic::k_assign_xor,         vxio::type::assign_t,     0x100C40000,  vxio::type::distance::assign,     lexem::k_assign_xor,  1},
                                              {mnemonic::k_assign_c1,          vxio::type::assign_t,     0x100C40000,  vxio::type::distance::assign,     lexem::k_assign_c1,     1},
                                              {mnemonic::k_assign_left_shift,  vxio::type::assign_t,     0x100C40000,  vxio::type::distance::assign,     lexem::k_assign_lshift, 1},
                                              {mnemonic::k_assign_right_shift, vxio::type::assign_t,     0x100C40000,  vxio::type::distance::assign,     lexem::k_assign_rshift, 1},
                                              {mnemonic::k_deref,              vxio::type::prefix_t,     0x000580000,  vxio::type::distance::deref,      lexem::k_deref        , 1},
                                              {mnemonic::k_less_eq,            vxio::type::binary_t,     0x200C00000,  vxio::type::distance::equality,   lexem::k_less_equal,    1},
                                              {mnemonic::k_greater_eq,         vxio::type::binary_t,     0x200C00000,  vxio::type::distance::equality,   lexem::k_greater_equal, 1},
                                              {mnemonic::k_equal,              vxio::type::binary_t,     0x200C00000,  vxio::type::distance::equality,   lexem::k_equal,        1},
                                              {mnemonic::k_not_equal,          vxio::type::binary_t,     0x200C00000,  vxio::type::distance::equality,   lexem::k_not_equal,    1},
                                              {mnemonic::k_not_equal,          vxio::type::binary_t,     0x200C00000,  vxio::type::distance::equality,   "<>",                      1},
                                              {mnemonic::k_add,                vxio::type::binary_t,     0x000C00000,  vxio::type::distance::addition,   lexem::k_addition,     1},
                                              {mnemonic::k_sub,                vxio::type::binary_t,     0x000C00000,  vxio::type::distance::addition,   lexem::k_sub,          1},
                                              {mnemonic::k_mul,                vxio::type::binary_t,     0x000C00000,  vxio::type::distance::product,    lexem::k_multiply,     1},
                                              {mnemonic::k_comment_cpp,        vxio::type::punc_t,       0x00000,      vxio::type::distance::noop_,      lexem::k_comment_cpp,  0},
                                              {mnemonic::k_modulo,             vxio::type::binary_t,     0x000C00000,  vxio::type::distance::product,    lexem::k_modulo,       1},
                                              {mnemonic::k_less_than,          vxio::type::binary_t,     0x200C00000,  vxio::type::distance::equality,   lexem::k_less_than,    1},
                                              {mnemonic::k_greater_than,       vxio::type::binary_t,     0x200C00000,  vxio::type::distance::equality,   lexem::k_greater_than, 1},
                                              {mnemonic::k_assign,             vxio::type::assign_t,     0x100C40000,  vxio::type::distance::assign,     lexem::k_assign,     1},
                                              {mnemonic::k_binary_and,         vxio::type::binary_t,     0x000C00000,  vxio::type::distance::bits,       lexem::k_binary_and,  1},
                                              {mnemonic::k_binary_or,          vxio::type::binary_t,     0x000C00000,  vxio::type::distance::bits,       lexem::k_binary_or,   1},
                                              {mnemonic::k_c2,                 vxio::type::prefix_t,     0x000580000,  vxio::type::distance::prefix,     lexem::k_c2,          1},
                                              {mnemonic::k_c1,                 vxio::type::prefix_t,     0x000580000,  vxio::type::distance::prefix,     lexem::k_c1,            1},
                                              {mnemonic::k_binary_not,         vxio::type::unary_t,      0x000580000,  vxio::type::distance::prefix,     lexem::k_not,           1},
                                              {mnemonic::k_logical_and,        vxio::type::binary_t,     0x200C00000,  vxio::type::distance::bool_and,   lexem::k_bool_and,      1},
                                              {mnemonic::k_logical_or,         vxio::type::binary_t,     0x200C00000,  vxio::type::distance::bool_or,    lexem::k_bool_or,       1},
                                              {mnemonic::k_open_par,           vxio::type::open_pair_t,  0x0880400000,  vxio::type::distance::paranthese, lexem::k_open_par,     1},
                                              {mnemonic::k_close_par,          vxio::type::close_pair_t, 0x1080400000, vxio::type::distance::paranthese, lexem::k_close_par,     1},
                                              {mnemonic::Openindex,            vxio::type::binary_t,     0x2080C00000, vxio::type::distance::paranthese, lexem::k_open_index,    1},
                                              {mnemonic::Closeindex,           vxio::type::close_pair_t, 0x4080400000, vxio::type::distance::paranthese, lexem::k_close_index,   1},
                                              {mnemonic::Openbrace,            vxio::type::binary_t,     0x0880C00000,  vxio::type::distance::paranthese, lexem::k_brace_begin,  1},
                                              {mnemonic::Closebrace,           vxio::type::close_pair_t, 0x1080400000, vxio::type::distance::paranthese, lexem::k_brace_end,     1},
                                              {mnemonic::k_begin_comment,      vxio::type::punc_t,       0x00000,      vxio::type::distance::noop_,      lexem::k_comment_begin, 0},
                                              {mnemonic::k_end_comment,        vxio::type::punc_t,       0x00000,      vxio::type::distance::noop_,      lexem::k_comment_end, 0},
                                              {mnemonic::k_div,                vxio::type::binary_t,     0x000C00000,  vxio::type::distance::product,    lexem::k_division,  1},
                                              {mnemonic::k_comma,              vxio::type::binary_t,     0x080C00000,  vxio::type::distance::comma,      lexem::k_comma,     1},
                                              {mnemonic::k_scope,              vxio::type::binary_t,     0x080C00000,  vxio::type::distance::scope,      lexem::k_scope,     1},
                                              {mnemonic::k_semicolon,          vxio::type::punc_t,       0x080000000,  vxio::type::distance::noop_,      lexem::k_semicolon, 0},
                                              {mnemonic::k_colon,              vxio::type::binary_t,     0x080C00000,  vxio::type::distance::noop_,      lexem::k_colon,     0},
                                              {mnemonic::k_range,              vxio::type::binary_t,     0x080C00000,  vxio::type::distance::scope,      lexem::k_range,     1},
                                              {mnemonic::k_factorial,          vxio::type::postfix_t,    0x000580000,  vxio::type::distance::unary,      lexem::k_factorial, 1},
                                              {mnemonic::k_positive,           vxio::type::prefix_t,     0x000580020,  vxio::type::distance::unary,      lexem::k_positive, 1},
                                              {mnemonic::k_negative,           vxio::type::prefix_t,     0x000580020,  vxio::type::distance::unary,      lexem::k_negative, 1},
                                              {mnemonic::k_squote,             vxio::type::text_t,       0x000400012,  vxio::type::distance::identifier, lexem::k_squote,  1},
                                              {mnemonic::k_dquote,             vxio::type::text_t,       0x000400012,  vxio::type::distance::identifier, lexem::k_dquote,  1},
                                              {mnemonic::k_ternary,            vxio::type::keyword_t,    0x000C40000,  vxio::type::distance::unary,      lexem::k_ternary, 1},
                                              {mnemonic::k_hash,               vxio::type::prefix_t,     0x000580000,  vxio::type::distance::unary,      lexem::k_hash,   1},
                                              {mnemonic::k_eos,                vxio::type::prefix_t,     0x000580000,  vxio::type::distance::unary,      lexem::k_eos,    1},
                                              {mnemonic::k_dot,                vxio::type::punc_t,       0x080400000,  vxio::type::distance::scope,      lexem::k_dot,    1},
                                              {mnemonic::k_return,             vxio::type::keyword_t,    0x000040000,  vxio::type::distance::identifier, lexem::k_return, 0},
                                              {mnemonic::k_if,                 vxio::type::keyword_t,    0x000040000,  vxio::type::distance::identifier, lexem::k_if,     0},
                                              {mnemonic::k_pi,                 vxio::type::keyword_t,    0x000044011,  vxio::type::distance::identifier, lexem::k_pi,     1},
                                              {mnemonic::k_number,             vxio::type::prefix_t,     0x000540001,  vxio::type::distance::identifier, lexem::k_number, 1},
                                              {mnemonic::k_u8,                 vxio::type::prefix_t,     0x000540400,  vxio::type::distance::identifier, lexem::k_u8,     1},
                                              {mnemonic::k_u16,                vxio::type::prefix_t,     0x000540800,  vxio::type::distance::identifier, lexem::k_u16,    1},
                                              {mnemonic::k_u32,                vxio::type::prefix_t,     0x000541000,  vxio::type::distance::identifier, lexem::k_u32,    1},
                                              {mnemonic::k_u64,                vxio::type::prefix_t,     0x000542000,  vxio::type::distance::identifier, lexem::k_u64,     1},
                                              {mnemonic::k_i8,                 vxio::type::prefix_t,     0x000540040,  vxio::type::distance::identifier, lexem::k_i8,      1},
                                              {mnemonic::k_i16,                vxio::type::prefix_t,     0x000540080,  vxio::type::distance::identifier, lexem::k_i16,     1},
                                              {mnemonic::k_i32,                vxio::type::prefix_t,     0x000540100,  vxio::type::distance::identifier, lexem::k_i32,     1},
                                              {mnemonic::k_i64,                vxio::type::prefix_t,     0x000540200,  vxio::type::distance::identifier, lexem::k_i64,     1},
                                              {mnemonic::k_float,              vxio::type::prefix_t,     0x000544000,  vxio::type::distance::identifier, lexem::k_float,   1},
                                              {mnemonic::k_string,             vxio::type::prefix_t,     0x000540002,  vxio::type::distance::identifier, lexem::k_string,  1},
                                              {mnemonic::k_then,               vxio::type::keyword_t,    0x000040000,  vxio::type::distance::identifier, lexem::k_then,    0},
                                              {mnemonic::k_else,               vxio::type::keyword_t,    0x000040000,  vxio::type::distance::identifier, lexem::k_else,    0},
                                              {mnemonic::k_const,              vxio::type::keyword_t,    0x2000040000, vxio::type::distance::identifier, lexem::k_const,   0},
                                              {mnemonic::k_include,            vxio::type::keyword_t,    0x000040000,  vxio::type::distance::identifier, lexem::k_include, 0},
                                              {mnemonic::k_module,             vxio::type::keyword_t,    0x000040000,  vxio::type::distance::identifier, lexem::k_module, 0},
                                              {mnemonic::k_at,                 vxio::type::prefix_t,     0x000580000,  vxio::type::distance::unary,      lexem::k_at,      1},
                                              {mnemonic::k_prime,              vxio::type::prefix_t,     0x000580000,  vxio::type::distance::unary,      lexem::k_prime,   1},
                                              {mnemonic::k_do,                 vxio::type::keyword_t,    0x000040000,  vxio::type::distance::noop_,      lexem::k_do,      0},
                                              {mnemonic::k_while,              vxio::type::keyword_t,    0x000040000,  vxio::type::distance::noop_,      lexem::k_while,       0},
                                              {mnemonic::k_for,                vxio::type::keyword_t,    0x000040000,  vxio::type::distance::noop_,      lexem::k_for,         0},
                                              {mnemonic::k_until,              vxio::type::keyword_t,    0x000040000,  vxio::type::distance::noop_,      lexem::k_until,       0},
                                              {mnemonic::k_repeat,             vxio::type::keyword_t,    0x000040000,  vxio::type::distance::noop_,      lexem::k_repeat,      0},
                                              {mnemonic::k_switch,             vxio::type::keyword_t,    0x000040000,  vxio::type::distance::noop_,      lexem::k_switch,      0},
                                              {mnemonic::k_case,               vxio::type::keyword_t,    0x000040000,  vxio::type::distance::noop_,      lexem::k_case,        0},
                                              {mnemonic::k_type,               vxio::type::keyword_t,    0x000040000,  vxio::type::distance::noop_,      lexem::k_type,        0},
                                              {mnemonic::k_low_hex,            vxio::type::hex_t,        0x400440011,  vxio::type::distance::noop_,      lexem::k_low_hex,     1},
                                              {mnemonic::k_up_hex,             vxio::type::hex_t,        0x400440011,  vxio::type::distance::noop_,      lexem::k_up_hex,      1},
                                              {mnemonic::k_cosinus,            vxio::type::prefix_t,     0x000580000,  vxio::type::distance::unary,      lexem::k_cosinus,     1},
                                              {mnemonic::k_arc_cosinus,        vxio::type::prefix_t,     0x0005C0000,  vxio::type::distance::unary,      lexem::k_arc_cosinus, 1},
                                              {mnemonic::k_tangent,            vxio::type::prefix_t,     0x0005C0000,  vxio::type::distance::unary,      lexem::k_tangent,     1},
                                              {mnemonic::k_arc_tangent,        vxio::type::prefix_t,     0x0005C0000,  vxio::type::distance::unary,      lexem::k_arc_tangent, 1},
                                              {mnemonic::k_sinus,              vxio::type::prefix_t,     0x0005C0000,  vxio::type::distance::unary,      lexem::k_sinus,       1},
                                              {mnemonic::k_arc_sinus,          vxio::type::prefix_t,     0x0005C0000,  vxio::type::distance::unary,      lexem::k_arc_sinus,   1},
                                              {mnemonic::k_object,             vxio::type::keyword_t,    0x000040000,  vxio::type::distance::identifier, lexem::k_object,      0},
                                              {mnemonic::k_static,             vxio::type::keyword_t,    0x000040000,  vxio::type::distance::identifier, lexem::k_static,      0},
                                              {mnemonic::k_this,               vxio::type::keyword_t,    0x000040000,  vxio::type::distance::identifier, lexem::k_this,        1}};

// It was the right one ... duh...
token_data token_data::scan(const char *C_)
{
    int      unicode = 0;
    int      index   = 0;
    //Rem::Debug() << __PRETTY_FUNCTION__ << ":\n";
    for(auto Tok: tokens_table)
    {
        const char *crs  = C_;
        const char *rtxt = Tok.mLoc.begin;
        std::size_t sz = std::strlen(rtxt);
        
        if(*crs != *Tok.mLoc.begin)
        {
            ++index;
            continue;
        }
        ++index;
        /*
        * offset arbitrary assume that the UNICODE UNIT is a signed 16 bits AND THAT THE VALUE OF FIRST BYTE IS NEGATIVE.
        * Then offset take the next byte to determine the unicode ...code...
        */
        while((*crs && *rtxt) && (*crs == *rtxt))
        {
            ////std::cout << *crs <<  *rtxt << ">>>";
            if(*crs < 0)
                ++unicode;
            ++crs;
            ++rtxt;
        }
        
        if(*rtxt == 0)
        {
            //            Rem::Debug() << "Check Token mnemonic :[" << MnemonicName(Tok.m) << ']';
            if(*crs && !isspace(*crs))
            {
                ///@todo DEBUG! Temporary boolean validation solution... Must be beaten very hard here.
                if((isalnum(*crs) || (*crs == '_')) && !Tok.is_operator())
                    continue;
            }
            
            Tok.mLoc.begin = C_;
            Tok.mLoc.end   = crs - 1;
            return Tok;
        }
    }
    return token_data::mNull;
    
}

vxio::type::T token_data::ktype(mnemonic m)
{
    return key_to_type[m];
}

token_data token_data::operator[](mnemonic CC)
{
    for(token_data T: tokens_table)
    {
        if(CC == T.c)
            return T;
    }
    return token_data::mNull;
}

std::string token_data::details(bool Mark_) const
{
    iostr Str = "'%s'[%s]: offset: %d line:%d, col:%d, %s/{%s}";
    Str << text() << mnemonic_name(c) << mLoc.offset << mLoc.linenum << mLoc.colnum << type_name() << semantic_types();
    if(Mark_)
        Str << '\n' << mark();
    return Str();
}

}
