//
// Created by slussier on 21-08-08.
//

#include <vxio/lexer/lexer.h>
#include <array>
#include <vxio/util/logger.h>

// Allright! VS2022 preview3 is broken!
namespace vxio
{

lexer::ScanTable  lexer::scan_table;

#pragma region InternalCursor

/*!
 * @brief Skips white spaces character, advancing(/consuming) m pointer
 *
 * prefix inc//Rement operator
 * @return true if m is not on EOF, false otherwise.
 */
bool lexer::internal_cursor::operator++()
{
    if(C >= E)
        return false;
    ++C;
    while((C < E) && (isspace(*C)))
        ++C;
    return true;
}

/*!
 * @brief Skips white spaces character, advancing(/consuming) m pointer
 *
 * Postfix inc//Rement operator, just calls the prefix inc//Rement operator.
 * @return true if m is not on EOF, false otherwise.
 */
bool lexer::internal_cursor::operator++(int)
{
    return ++(*this);
}

/*!
* @brief Skips white spaces character, advancing(/consuming) m pointer
*
* Named method, just calls the prefix increment operator.
* @return true if m is not on EOF, false otherwise.
*/
[[maybe_unused]] bool lexer::internal_cursor::skip_ws()
{
    while((C <= E) && (isspace(*C)))
    {
        ++Col;
        if(*C == '\n' || *C == '\r')
        {
            ++L;
            Col = 1;
        }
        
        ++C;
    }
    return C <= E;
}

/*!
 * @brief Tests if P is on or past EOF.
 * @param P
 * @return true if P is EOF, false otherwise.
 */
bool lexer::internal_cursor::end_of_file(const char *P) const
{
    if(P)
        return P > E;
    return C > E;
}

/*!
 * @brief Synchronize the Location data from the IC pointer.
 *
 * @return none.
 */
void lexer::internal_cursor::sync()
{
    const char *C_;
    L   = 1;
    Col = 1;
    C_  = B;
    while((C_ >= B) && (C_ <= E) && (C_ < C))
    {
        if((*C_ == '\n') || (*C_ == '\r'))
        {
            Col = 1;
            L++;
        }
        else
            ++Col;
        ++C_;
    }
    
    //rem::codeDebug(__PRETTY_FUNCTION__) << ": " << Location();
}

/*!
 * @brief Get the ptrdiff between the m pointer and the beginning of the source text (B pointer).
 * @return int.
 */
long lexer::internal_cursor::Index() const
{
    return (ptrdiff_t) (C - B);
}

/*!
 * @brief Advances/Consume the m pointer till the next NewLine{'\r'; '\n'}  code in the source text
 * @return distinct std::string of the sequence.
 */
[[maybe_unused]] std::string lexer::internal_cursor::scan_to_eol()
{
    std::string Str;
    while((C <= E) && (*C != '\n') && (*C != '\r'))
        Str += *C++;
    return Str;
}

/*!
 * @brief Get a std::string copy of the current line from the m pointer
 * @return string.
 */
std::string lexer::internal_cursor::line_num() const
{
    std::string Str;
    Str.clear();
    
    const char *lb, *eb;
    lb = eb = C;
    
    while((eb <= E) && (*eb != '\r') && (*eb != '\n'))
        ++eb;
    while((lb > B) && (*lb != '\r') && (*lb != '\n'))
        --lb;
    
    for(; lb < eb; lb++)
        Str += *lb;
    return Str;
}

/*!
 * @brief Build a line iostr from the current position then mark at the current columns.
  * @return std::string
  * @note : Must be Sync()'ed before calling mark();
 
 */

std::string lexer::internal_cursor::mark() const
{
    iostr Str;
    Str << L << "," << Col << " =>\n";
    for(int x = 1; x < Col; x++)
        Str << ' ';
    Str << '^';
    return Str();
}

/*!
 * @brief Get the string representation of the [internal]cursor location in the source text.
 * @return std::string
 */
[[maybe_unused]] std::string lexer::internal_cursor::location() const
{
    iostr Str = "%d,%d";
    Str << L << Col;
    return Str();
}

/*!
 * @brief Advances/Consumes the m pointer to Skip till SubStr_ match.
 * @param SubStr_
 * @return Expect code.
 */
[[maybe_unused]] rem::code lexer::internal_cursor::scan_to(const char *SubStr_)
{
    
    return rem::code::ok; //rem::codeInt::Ok;
}

std::string lexer::internal_cursor::scan_string()
{
    //rem::codeDebug(__PRETTY_FUNCTION__ +':'+ __LINE__) << '\n';
    const char  *be    = C;
    char        Quote_ = *be;
    std::string Str;
    Str += *be;
    ++be;
    
    Continue:
    while((be <= E) && (*be != '\\') && (*be != Quote_))
        Str += *be++;
    if(*be == '\\')
    {
        Str += *be++;
        Str += *be++; // Assume no terminal escape sequence here!!!!! PLEASE be a quote!!!
        goto Continue;
    }
    
    if((*be != Quote_) && (be > E))
    {
        sync();
        logger::error() << " : Unterminated string constant:\n" << mark();
        return "eof";
    }
    Str += *be; // Include the rhs Quote.
    return Str;
}

lexer::internal_cursor::internal_cursor(const char *Source_)
{
    C = Source_;
    E = C + std::strlen(C) - 1;
    B = C;
    L = Col = 1;
}

#pragma endregion InternalCursor

#pragma region NumScanner

lexer::num_scanner::num_scanner(const char *_c, const char *_eos) : B(_c), C(_c), E(nullptr), Eos(_eos)
{}

/*!
 * @brief For now a bare minimum digit with some rough floating point scan.
 * @return true if the m pointer is consumed and advanced
 */
bool lexer::num_scanner::operator++(int)
{
    if(C > Eos)
        return false;
    
    if(!isdigit(*C))
    {
        if(*C == '.')
            if(!Real)
            {
                Real = true;
            }
            
            else
                return false;
        else
            return false;
    }
    
    E = C;
    ++C;
    if((C <= Eos) && isdigit(*C))
        E = C;
    return C < Eos;
}

/*!
 * @brief Implements boolean operator
 * @return true if this NumScanner was  a valid numeric sequence, false otherwise.
 */
lexer::num_scanner::operator bool() const
{
    return E >= B;
}

/*!
 * @brief Computes the numeric scale and 'best guess' base.
 * @return one [combined] of {{u,i}{8,16,32,64}} | fp| oct | hex | bin.
 *
 * @note Numeric Base is omitted as of this version. Thus it only computes the Scale.
 */
vxio::type::T lexer::num_scanner::operator()() const
{
    if(!Real)
    {
        iostr    Str = iostr::make_str(B, E);
        uint64_t D;
        Str >> D;
        uint64_t                I = 0;
        std::array<uint64_t, 3> R = {0x100, 0x10000, 0x100000000};
        while(D >= R[I])
            ++I;
        std::array<vxio::type::T, 4> Capacity = {vxio::type::u8_t, vxio::type::u16_t, vxio::type::u32_t, vxio::type::u64_t};
        vxio::type::T                atoken   = Capacity[I];
        return atoken;
    }
    
    return vxio::type::float_t;
}

#pragma endregion NumScanner

#pragma region Scanners



// + 4ac(x^2+y+b)


lexer::Scanner lexer::get_scanner(token_data &token)
{
    for(const auto &scanners_tbl: lexer::scan_table)
    {
        if(token.s & scanners_tbl.first)
            return scanners_tbl.second;
    }
    return nullptr;//rem::codeDebug(__PRETTY_FUNCTION__) << " Not yet implemented";
}

rem::code lexer::input_binary_operator(token_data &token)
{
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";
    if(token.c == mnemonic::k_sub || token.c == mnemonic::k_add)
    {
        if(scan_sign_prefix(token) == rem::code::accepted)
            return rem::code::accepted;
    }
    Push(token);
    src_cursor._F = false;
    return rem::code::accepted;
}

/*!
 * @brief Unknown Input Token (Either number_t litteral or identifier).
 * @return Expect<>
 */
rem::code lexer::input_default(token_data &atoken)
{
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";
    if(scan_number(atoken) != rem::code::accepted)
    {
        //rem::codeDebug() << " Not a number_t Trying scan_identifier:";
        if(scan_identifier(atoken) != rem::code::accepted)
        {
            //rem::codePush() << mCursor.mark() << ":";
            return rem::code::rejected;//rem::codePush() << //rem::codevxio::type::Fatal << ": " << //rem::codeInt::UnExpected << " Token type " << atoken.type_name();
        }
    }
    
    return rem::code::accepted; // return rem::code::rejected  --- duh?
}

rem::code lexer::input_unary_operator(token_data& atoken)
{
    
    // Possible prefix and Postfix unary operators:
    if((atoken.c == mnemonic::k_binary_not) || (atoken.c == mnemonic::k_decr) || (atoken.c == mnemonic::k_incr))
    {
        if(mConfig.Tokens->empty() || (mConfig.Tokens->back().s & vxio::type::binary_t))
            return scan_prefix(atoken);
        return scan_postfix(atoken);
    }
    if(atoken.t == vxio::type::prefix_t)
        return scan_prefix(atoken);
    return scan_postfix(atoken);
}

rem::code lexer::input_punctuation(token_data &atoken)
{
    if(atoken.c == mnemonic::k_open_par)
    {
        if(src_cursor._F)
        {
            Push(atoken);
            insert_multiply(atoken);
            src_cursor._F = false;
            return rem::code::accepted;
        }
        // La seule et unique condition est que le token precedant soit une valeur numerique litterale (ex.: '4').
        if(!mConfig.Tokens->empty())
        {
            if((mConfig.Tokens->back().s & type::const_t|type::number_t) && (mConfig.Tokens->back().t != type::id_t) && !(mConfig.Tokens->back().s & type::operator_t))
            {
                Push(atoken);
                insert_multiply(atoken);
                src_cursor._F = false;
                return rem::code::accepted;
            }
        }
    }
    if(atoken.c == mnemonic::k_dot)
        return scan_number(atoken);
    
    if(atoken.c == mnemonic::k_comment_cpp)
        return skip_cpp_comment();
    
    return Push(atoken);
}

rem::code lexer::input_keyword(token_data &atoken)
{
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";
    return Push(atoken);
}

rem::code lexer::input_hex(token_data &atoken)
{
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";
    //rem::codeDebug(__PRETTY_FUNCTION__) << ":\n";
    const char *C_ = src_cursor.C;
    C_ += atoken.text().length();
    const char *E_ = C_;
    if(isspace(*E_))
        return rem::code::rejected;
    
    while(*E_ && !isspace(*E_) &&
          (
              ((*E_ >= '0') && (*E_ <= '9')) ||
              ((*E_ >= 'A') && (*E_ <= 'F')) ||
              ((*E_ >= 'a') && (*E_ <= 'f'))
          )
        )
        ++E_;
    if(E_ > C_)
        --E_;
    
    atoken.mLoc.end = E_;
    atoken._flags.V = 1;
    return Push(atoken);
}

/*!
 * @brief Scans const numeric constrtuct
 * @param atoken
 * @return rem::codeaccepted;
 * @todo scan Scientific Notation!!!
 */
rem::code lexer::scan_number(token_data &atoken)
{
    
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";
    num_scanner Num = num_scanner(src_cursor.C, src_cursor.E);
    while(Num++);
    if(!Num.operator bool())
        return rem::code::rejected;
    
    if(src_cursor._F)
        src_cursor._F = false;
    
    atoken.t          = vxio::type::leaf_t;
    atoken.s          = vxio::type::number_t | Num() | vxio::type::leaf_t;
    atoken._flags.V   = 1;
    atoken.mLoc.begin = Num.B;
    atoken.mLoc.end   = Num.E; // And Num.C ?
    //rem::codeDebug() << "lexer::scan_number: Cursor on \n" << mCursor.mark();
    if(!(atoken.s & vxio::type::float_t))
    {
        iostr str;
        str << atoken.text();
        uint64_t           D = 0;
        std::istringstream i(str.c_str());
        switch(Num.Num)
        {
            case num_scanner::Bin:
                //????????? ah!
                break;
            case num_scanner::Oct:i >> std::oct >> D;
                break;
            case num_scanner::Dec:i >> D;
                break;
            case num_scanner::Hex:i >> std::hex >> D;
                break;
            default:str >> D;
                break;
        }
        
        //std::cout << __PRETTY_FUNCTION__ << " Parsed number:" << D << '\n';
        uint64_t                n = 0;
        std::array<uint64_t, 3> R = {0x100, 0x10000, 0x100000000};
        while(D >= R[n])
            ++n;
        
        switch(n)
        {
            case 0:atoken.s = (atoken.s & ~vxio::type::u64_t) | vxio::type::u8_t;
                break;
            case 1:atoken.s = (atoken.s & ~vxio::type::u64_t) | vxio::type::u16_t;
                break;
            case 2:atoken.s = (atoken.s & ~vxio::type::u64_t) | vxio::type::u32_t;
                break;
            default:atoken.s |= vxio::type::u64_t;
                break;
        }
    }
    atoken.c          = mnemonic::noop_;
    atoken.s |= type::const_t;
    return Push(atoken);
    //return rem::codeaccepted;
}

rem::code lexer::scan_identifier(token_data &atoken)
{
    
    //rem::codeDebug(__PRETTY_FUNCTION__);
    const char *C = src_cursor.C;
    if((!isalpha(*C)) && (*C != '_'))
        return rem::code::rejected;
    
    if(src_cursor._F)
        goto IDOK;
    else
    {
        if(!mConfig.Tokens->empty())
        {
            if(mConfig.Tokens->back().s & (vxio::type::number_t))
            {
                src_cursor._F = true;
                goto IDOK;
            }
        }
    }
    while(*C && (isalnum(*C) || (*C == '_')))
        ++C;
    --C;
    IDOK:
    atoken.mLoc.begin   = src_cursor.C;
    atoken.mLoc.end     = C;
    atoken.t            = vxio::type::id_t;
    atoken.s            = vxio::type::id_t | vxio::type::leaf_t;
    atoken.c            = mnemonic::noop_;
    atoken.mLoc.linenum = src_cursor.L;
    atoken.mLoc.colnum  = src_cursor.Col;
    atoken._flags.V     = src_cursor._F ? 1 : 0; //Subject to be modified
    Push(atoken);
    if(src_cursor._F)
        insert_multiply(atoken);
    
    return rem::code::accepted;
}

void lexer::insert_multiply(token_data &atoken)
{
    token_data Mul;
    Mul = atoken; // Push atoken properties in the incoming virtual multiply operator
    Mul.t        = vxio::type::binary_t;
    Mul.s        = vxio::type::binary_t | vxio::type::operator_t;
    Mul._flags.M = Mul._flags.V = 1;
    Mul.c        = mnemonic::k_mul;
    auto i = --mConfig.Tokens->end();
    mConfig.Tokens->insert(i, Mul);
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n Details:" << Mul.details() << "\n" << Mul.mark();
}

/*!
 * @brief  Scans for std maths factor notation, RESTRICTED (limited) syntax style:
 *         4ac => 4 x a x m
 *         4(ac...) != 4 x ( a x m ...)
 *         4pi/sin/cos/atan/asin/acos ... != 4 x p x i / 4 x s x i x n ... And NOT 4 x pi or 4 x sin ...
 * *
 *
 * @note   Required that the Left hand side token is a ContNumber and that the Input token is contiguous and of unknown type (vxio::type::null_t) to be scanned as an identifier.
 *         Input atoken is either scanned in the ref_t Table or not.
 * @return Execp<>
 *
rem::code lexer::ScanFactorNotation(token_data &atoken)
{
    // Tokens stream is NOT EMPTY here.
    
    // Required that the next atoken is contiguous ( no [white]space between lhs and atoken ).
    //rem::codeDebug() << __PRETTY_FUNCTION__ << ":";
    token_data k_mul;
    if((mCursor.colnum - mConfig.Tokens->back().mLoc.end) != (ptrdiff_t) 1)
    {
        //rem::codeDebug() << "No factor notation style seq:[ptrdiff_t:" << mCursor.colnum - mConfig.Tokens->back().mLoc.end << "]:\n" << mCursor.mark();
        //rem::codeDebug() << "lexer::ScanFactorNotation: mCursor:" << mCursor.colnum - mCursor.B << " <::> Tail[begin]:" << mConfig.Tokens->back().mLoc.begin - mCursor.B;
        return //rem::codeFatal() << //rem::codeInt::Expected << " Factor notation style sequence.";
    }
    // Set _F "state" flag :
    if(!mCursor._F)
    {
        // LHS is Restricted to number_t, triggering the Factor notation sequence flag.
        if(!mConfig.Tokens->back().is_number())
            return //rem::codeFatal() << //rem::codeInt::Expected << " Factor notation style sequence.";
    }
    
    // Expecting RHS to be an identifier Token
    if(*scan_identifier(atoken) != rem::codeaccepted)
        return //rem::codeInt::Rejected;
    
    // triggering the Factor notation sequence flag.
    mCursor._F = true;
    
    atoken.mLoc.end = atoken.mLoc.begin; // Adjust (CUT) the identifier Attribute to ONE char.
    ////rem::codeDebug() << "Confirmed lexer::ScanFactorNotation: " << atoken.details(true);
    k_mul = atoken; // Push atoken properties in the incoming virtual multiply operator
    k_mul.T        = vxio::type::Binary;
    k_mul.s        = vxio::type::Binary | vxio::type::Operator;
    k_mul.mFlags.m = k_mul.mFlags.V = 1;
    k_mul.m        = mnemonic::k_mul;
    mCursor.Sync();
    k_mul.mLoc.linenum = mCursor.linenum;
    k_mul.mLoc.colnum = mCursor.Col;
    mConfig.Tokens->push_back(k_mul);
    mCursor.colnum = k_mul.mLoc.begin;
    return Push(atoken);
}
*/

rem::code lexer::scan_sign_prefix(token_data &atoken)
{
    if(!mConfig.Tokens->empty() && (mConfig.Tokens->back().s & vxio::type::close_pair_t))
    {
        //logger::comment() << "lexer::scan_sign_prefix:\n" << atoken.mark() << "\n" << " rejected...\n";
        return rem::code::rejected;
    }
    
    if(mConfig.Tokens->empty() || mConfig.Tokens->back().is_binary() || mConfig.Tokens->back().is_punctuation())
    {
        atoken.t = vxio::type::prefix_t;
        atoken.s = (atoken.s & ~vxio::type::binary_t) | vxio::type::sign_t | vxio::type::unary_t | vxio::type::prefix_t; // vxio::type::Operator bit already set
        return Push(atoken);
    }
    return rem::code::rejected;
}

/*!
 * @brief  Operators such as '--', '++', '!' can be prefix or postfix unary operators.
 *
 * @return
 */
rem::code lexer::scan_prefix(token_data &atoken)
{
    return Push(atoken);
}

/*!
 * @brief  Operators such as '--', '++', '!' can be prefix or postfix unary operators.
 *
 * @return
 */
rem::code lexer::scan_postfix(token_data &atoken)
{
    if(!((atoken.c == mnemonic::k_decr) || (atoken.c == mnemonic::k_incr) || (atoken.c == mnemonic::k_binary_not)))
        return rem::code::rejected;
    
    atoken.t = vxio::type::postfix_t;
    atoken.s = (atoken.s & ~vxio::type::prefix_t) | vxio::type::postfix_t; // unary/Operator ...  already set.
    if(atoken.c == mnemonic::k_binary_not)
        atoken.c = mnemonic::k_factorial;
    
    //mCursor.Sync();
    return Push(atoken);
}

#pragma endregion Scanners

rem::code lexer::Push(token_data &atoken)
{
    if(!atoken)
        return rem::code::rejected;//rem::codePush() << //rem::codevxio::type::Error << ": Attempt to push a null_t token_data into the Tokens stream.";
    
    //mCursor.Sync();
    atoken.mLoc.linenum = src_cursor.L;
    atoken.mLoc.colnum  = src_cursor.Col;
    
    std::size_t sz = atoken.mLoc.end - atoken.mLoc.begin + 1;
    atoken.mLoc.offset = static_cast<ptrdiff_t>(atoken.mLoc.begin - src_cursor.B);
    src_cursor.C += sz;
    src_cursor.Col += sz;
    mConfig.Tokens->push_back(atoken);
    src_cursor.skip_ws();
    //rem::codeDebug(__PRETTY_FUNCTION__) << "lexer::Push: Size of Token:" << sz << ", token_data " << atoken.details(true);
    //rem::codeDebug() << "Cursor(Next Token):" << mCursor.Location() << '\n' << mCursor.mark();
    return rem::code::accepted;
}

rem::code lexer::operator()()
{
    return Exec();
}

void lexer::dump_tokens(std::function<void(const token_data &)> callback_)
{
    if(!callback_)
        return;
    for(const auto &token: *mConfig.Tokens)
        callback_(token);
}

rem::code lexer::input_text(token_data &atoken)
{
    //logger::debug() << __PRETTY_FUNCTION__ << ":\n";
    
    std::string R = src_cursor.scan_string();
    //@todo Separate this token into three : {quote, text, quote}
    
    if(R.empty())
        return rem::code::rejected;
    
    // We separate tokens here ...
    Push(atoken);
    //...
    const char* e = src_cursor.C + (R.length()-2);  // oops!
    atoken.mLoc.begin = src_cursor.C;
    atoken.mLoc.end = e-1;
    atoken.c = mnemonic::noop_;
    atoken.t = type::text_t;
    atoken.s = type::text_t|type::leaf_t|type::const_t;
    Push(atoken);

    atoken.mLoc.begin = atoken.mLoc.end = e;
    atoken.t = type::text_t;
    atoken.s = type::text_t|type::leaf_t|type::operator_t;
    atoken.c = *e == '\'' ? mnemonic::k_squote : mnemonic::k_dquote;
    
    Push(atoken);
    return rem::code::accepted;
}

rem::code lexer::Exec()
{
    rem::code R;
    if(!mConfig)
        return rem::code::rejected; // Use logger::push_error to queu error message and code...
    //...
    if(lexer::scan_table.empty())
    {
        lexer::scan_table = {
            {vxio::type::null_t, &lexer::input_default},
            {vxio::type::binary_t,    &lexer::input_binary_operator},
            {vxio::type::hex_t, &lexer::input_hex},
            {vxio::type::punc_t, &lexer::input_punctuation},
            {vxio::type::prefix_t, &lexer::scan_prefix},
            {vxio::type::keyword_t, &lexer::input_keyword},
            {vxio::type::text_t, &lexer::input_text}
            //...
        };
    }
    
    token_data atoken;
    
    src_cursor = lexer::internal_cursor(mConfig.Source);
    src_cursor.skip_ws();
    //rem::codeDebug() << "lexer::Exec(): Scanning '" << mCursor.colnum << "':\n";
    
    const char *C = nullptr;
    while(!src_cursor.end_of_file())
    {
        if(C == src_cursor.C)
        {
            logger::error() << "lexer: internal loop on\n" << src_cursor.mark() << "\n";
            return rem::code::rejected;
        }
        
        C = src_cursor.C;
        
        atoken = token_data::scan(src_cursor.C);
        //logger::debug() << __PRETTY_FUNCTION__ << ": " << atoken.details();
        Scanner S = get_scanner(atoken);
        if(S)
        {
            
            if((this->*S)(atoken) != rem::code::accepted)
            {
                logger::fatal() << "lexer: aborted: unexpected scan rejection at position:\n" << src_cursor.mark() << '\n';
                return rem::code::rejected;//
            }
        }
        else
        {
            if(atoken.c == mnemonic::k_comment_cpp)
            {
                skip_cpp_comment();
                continue;
            }
            logger::error() << "lexer loop: there is no scanner for token:\n" << src_cursor.mark();
            return rem::code::rejected;
        }
        
    }
    return rem::code::accepted;//rem::codeInt::Ok;
}

rem::code lexer::step_begin()
{
    
    if(!mConfig)
        return rem::code::rejected;//rem::codeFatal() << "lexer::Exec(): Config Data is missing crucial informations...";
    
    if(lexer::scan_table.empty())
    {
        lexer::scan_table = {
            {vxio::type::null_t, &lexer::input_default},
            {vxio::type::binary_t,  &lexer::input_binary_operator},
            {vxio::type::hex_t, &lexer::input_hex},
            {vxio::type::punc_t, &lexer::input_punctuation},
            {vxio::type::prefix_t, &lexer::scan_prefix},
            {vxio::type::keyword_t, &lexer::input_keyword},
            {vxio::type::text_t, &lexer::input_text}
        };
    }
    src_cursor = lexer::internal_cursor(mConfig.Source);
    src_cursor.skip_ws();
    return rem::code::accepted;
}

static const char *C = nullptr;

token_data *lexer::step()
{
    if(src_cursor.end_of_file())
        return nullptr;//rem::codereturn_status() << rem::codeeof;
    
    
    if(C == src_cursor.C)
    {
        
        return nullptr;
    }
    
    C = src_cursor.C;
    
    token_data token;
    token = token_data::scan(src_cursor.C);
    //rem::codeDebug(__PRETTY_FUNCTION__) << " Details: " << atoken.details();
    Scanner S = get_scanner(token);
    if(S)
    {
        if((this->*S)(token) != rem::code::accepted)
            return nullptr;// status() << rem::coderejected << " -> Aborted: Unexpected token:\n" << src_cursor.mark();
    }
    else
        return nullptr;// status() << rem::code::rejected << " No scanner for token:\n" << src_cursor.mark();
    
    return &mConfig.Tokens->back(); // Ouch... 
}


rem::code lexer::skip_cpp_comment()
{
    src_cursor.scan_to_eol();
    src_cursor.skip_ws();
    return rem::code::accepted;
}

}
