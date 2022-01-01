//
// Created by oldlonecoder on 21-12-12.
//

#include <vxio/interpret/compiler/grammar.h>

namespace vxio
{
//
// Created by slussier on 19-08-25.
//




grammar::Dictionary grammar::grammar_dictionnary = {
    {':', &grammar::enter_rule_def},
    {'|', &grammar::new_sequence},
    {'.', &grammar::end_rule},
    {'+', &grammar::set_repeat},
    {'*', &grammar::set_optional},
    {'?', &grammar::set_oneof}, // One of
    {'\'', &grammar::enter_litteral},
    {'"', &grammar::enter_litteral},
    {'#', &grammar::set_directive},
};

rule::collection grammar::rules;
static bool      built                            = false;



std::string grammar_text = R"(
xio-unit           : 'unit' id_t;.
stmts              : +statement.
statement          : ';' | instruction | assignstmt ';'| declvar ';'| #expression ';'| #var_id ';'.
assignstmt         : declvar assign_t expression | #var_id assign_t #expression.
declvar            : *typename #newvar.
funcsig            : *typename function_id '(' *params ')'.
declfunc           : funcsig ';'| funcsig statement ';' | funcsig bloc.
paramseq           : ',' param.
param              : *typename id_t.
function_call      : function_id '(' args ')' | objcfncall.
params             : param *+paramseq.
objcarg            : id_t ':' #expression.
arg                : objcarg | #expression.
argseq             : ',' arg.
args               : arg *+argseq.
typename           : *'static' ?'i8' ?'u8' ?'i16' ?'u16' ?'i32' ?'u32' ?'i64' ?'u64' ?'real' ?'number' ?'string' ?#objectid.
instruction        : ?'if' ?'then'  ?'switch' ?'case' ?'for' ?'while' ?'repeat' ?'until' ?'do' ?'return'.
if                 : 'if' condexpr ifbody | 'if' '(' condexpr ')' ifbody.
bloc               :  '{' stmts '}'.
truebloc           : *'then' bloc | *'then' statement.
elsebloc           : 'else' bloc | 'else' statement.
ifbody             : truebloc *elsebloc.
condexpr           : assignstmt | #expression.
function_id        : *'::' #functionid | #objectid '::' #functionid | #obj_instance '.' #functionid.
objcfncall         : '[' function_id  *args ']'.

)";

grammar::grammar()
{
    
}

grammar::~grammar()
{
    

}

rem::code grammar::build()
{
    logger::info() << __PRETTY_FUNCTION__ << ":";
    if(built())
    {
        logger::debug() << "grammar is already built - ok";
        return rem::code::ok;
    }
    if(_text.empty())
        _text = grammar_text;
    
    logger::debug()
        << color::White << ":Building rules :\n----------------------------\n"
        << color::Yellow << _text
        << color::White << "\n----------------------------------\n";
    
    std::size_t   count = _text.words(tokens, ":.+*?#", true);
    iostr::list_t List;
    logger::debug() << "building words list...";
    for(auto s: tokens)
        List.push_back(s());
    
    if(!count)
    {
        logger::fatal() << ": grammar. (internal error)";
        return rem::code::empty;
    }
    
    logger::debug() << List.size() << " words.";
    auto s = List.begin();
    _state = st_begin;
    do
    {
        rem::code r;
        auto      p = grammar::grammar_dictionnary.find((*s)[0]);
        if(p != grammar::grammar_dictionnary.end())
        {
            r = (this->*(p->second))(s);
        }
        else
        {
            r = parse_identifier(s);
        }
        if(r != rem::code::accepted)
            return r;
    }
    while(s != List.end());
    logger::debug() << ":";
    // dump(); // disabled
    return rem::code::accepted;
}

std::string grammar::dump_sequence(const term_seq& seq)
{
    iostr Out;
    Out << color::White << " | ";// << Ends;
    for(auto t: seq.terms)
    {
        if(t.a.is_oneof())
            Out << color::Cornsilk1;
        else if(t.a.is_optional())
            Out << color::Wheat4;
        else if(t.a.is_strict())
            Out << color::Thistle3;
        else if(t.a.X)
            Out << color::Black;
        else
            Out << color::Black;

        switch(t._type)
        {
            case term::type::m:Out << color::LightCoral;
            break;
            case term::type::r:
            {
                if(t.a.X)
                    Out << color::White;
                else
                    Out << color::LightPink4;
            }
            break;
            default: break;
        }
        Out << t() << ' ';// << Ends;
    }
    Out << color::Reset;
    return Out();
}


void grammar::dump()
{
    
    logger::info() << color::LightCoral << "mnemonic" << color::White << ',' <<
                   color::LightPink4 << "rule" << color::White << ',' <<
                   color::LightCyan3 << "semantic" << color::White << ',' << color::Magenta6 << " Strict (implicit) rule:\n";
    iostr Out;
    for(const auto &rule: rules)
    {
        Out << color::LightPink4 << rule.second->_id << color::White << ':';
        
        for(auto seq: rule.second->sequences)
        {
            iostr SeqStr;
            SeqStr <<  dump_sequence(seq);
            Out << SeqStr;
        }
        Out << '\n';
    }
    logger::output() << rem::code::endl  << Out();
}

void grammar::init_rules()
{

}

rem::code grammar::parse_identifier(iostr::Iterator &crs)
{
    rule *r = query_rule(*crs);
    switch(_state)
    {
        case st_begin:
            if(r)
            {
                if(!r->empty())
                {
                    logger::fatal() << " rule, named: " << *crs << " already exists in the context of a new rule definition.";
                    return rem::code::rejected;
                }
                _rule = r;
            }
            else
            {
                _rule = new rule(*crs);
                rules[*crs] = _rule;
            }
            a.Reset();
            _state = st_init_rule; //  expect ':' as next token in main loop.
            break;
        case st_init_rule:_state = st_seq;
            break;
        case st_option:
        case st_seq:_state = st_seq;
            // lexem::T ?
            /*lexer::lexem::lexer::lexem::mnemonic m = lexem::m(crs->c_str());
            if( m != lexer::lexem::lexer::lexem::mnemonic::knull ) {
                _rule->a = a;
                (*_rule) | m;
                a.reset();
                break;
            }*/
            
            type::T t = type::from_str(*crs);
            if(t)// & teacc::type::bloc_t) // Quick and dirty hack about bypassing the lexer::teacc::type::bloc type:
            {
                _rule->a = a;
                (*_rule) | t;
                a.Reset();
                break;
            }
            
            if(r)
            {
                _rule->a = a;
                (*_rule) | r;
                a.Reset();
                break;
            }
            else
            {
                r = new rule(*crs);
                rules[*crs] = r;
                _rule->a = a;
                _state = st_seq; //  expect ':' as next token in main loop.
                (*_rule) | r;
                a.Reset();
            }
            break;
    }
    ++crs;
    return rem::code::accepted;
}

rem::code grammar::enter_rule_def(iostr::Iterator &crs)
{
    // ':' :
    if(_state == st_init_rule)
    {
        _state = st_seq;
        a.Reset();
        ++crs;
        return rem::code::accepted;
    }
    logger::fatal(src_long_funcname) << " misplaced ':' ";
    return rem::code::rejected;
}

rem::code grammar::new_sequence(iostr::Iterator &crs)
{
    // logdebug
    //     << color::HCyan << __FUNCTION__
    //     << color::White << ": ["
    //     << color::Yellow << *crs
    //     << color::White << ']'
    //     << Ends;
    
    if(_state == st_option)
    {
        logger::fatal() << " syntax error: " << *crs << "' is invalid in this context";
        return rem::code::rejected;
    }
    
    _rule->new_sequence();
    _state = st_seq;
    a.Reset();
    ++crs;
    return rem::code::accepted;
}

rem::code grammar::end_rule(iostr::Iterator &crs)
{
    // logdebug
    //     << color::HCyan << __FUNCTION__
    //     << color::White << ": ["
    //     << color::Yellow << *crs
    //     << color::White << ']'
    //     << Ends;
    _state = st_begin;
    ++crs;
    return rem::code::accepted;
}

rem::code grammar::set_repeat(iostr::Iterator &crs)
{
    //logdebug
    //    << color::HCyan << __FUNCTION__
    //    << color::White << ": ["
    //    << color::Yellow << *crs
    //    << color::White << ']'
    //    << Ends;
    _state = st_option;
    +a;
    ++crs;
    return rem::code::accepted;
}

rem::code grammar::set_directive(iostr::Iterator &crs)
{
    !a;
    _state = st_option;
    /*
               logger::debug()
               << color::Yellow << a()
               << color::White << ": ["
               << color::Yellow << *crs
               << color::White << ']';
   */
    
    ++crs;
    return rem::code::accepted;
}

rem::code grammar::set_optional(iostr::Iterator &crs)
{
    //logdebug
    //    << color::HCyan << __FUNCTION__
    //    << color::White << ": ["
    //    << color::Yellow << *crs
    //    << color::White << ']'
    //    << Ends;
    *a;
    ++crs;
    _state = st_option;
    return rem::code::accepted;
}

rem::code grammar::enter_litteral(iostr::Iterator &crs)
{
    
    //    logger::debug()
    //    << color::White << ": ["
    //    << color::Yellow << *crs
    //    << color::White << ']';
    
    if((_state != st_seq) && (_state != st_option))
    {
        logger::fatal() << "syntax error '" << *crs << "' is not a valid xio++ grammar token in context" << "(state machine:" << (int) _state << ")";
        return rem::code::rejected;
    }
    
    iostr::Iterator i, quote;
    i = quote = crs;
    ++i;
    if(*i == *quote)
    {
        logger::fatal() << "error: litteral grammar element cannot be empty";
        return rem::code::rejected;
    }
    
    token_data token = token_data::scan(i->c_str());
    if(token)
    {
        _rule->a = a;
        (*_rule) | token.c;
        a.Reset();
    }
    else
    {
        logger::fatal() << "syntax error '" << *i << "' is not a valid xio++ grammar token";
        return rem::code::rejected;
    }
    crs = i;
    ++crs;
    if(*crs == *quote)
        ++crs;
    //++crs; // will be on the next token.
    
    return rem::code::accepted;
}

rem::code grammar::set_oneof(iostr::Iterator &crs)
{
    //     logdebug
    //         << color::HCyan << __FUNCTION__
    //         << color::White << ": ["
    //         << color::Yellow << *crs
    //         << color::White << ']'
    //         << Ends;
    ~a;
    ++crs;
    return rem::code::accepted;
}

[[maybe_unused]] int grammar::init()
{
    _text = "";
    return 0;
}

rule *grammar::query_rule(const std::string &a_id)
{
    auto i = rules.find(a_id);
    return i == rules.end() ? nullptr : i->second;
}


const rule *grammar::operator[](const std::string &r_id) const
{
    logger::debug(src_long_funcname) << " query: '" << r_id << "':\n";
    auto i = grammar::rules.find(r_id);
    if(i == grammar::rules.end())
    {
        logger::error() << " no such rule: '" << r_id << "'";
        return nullptr;
    }
    logger::debug() << " rule '" << r_id << "' found.";
    return i->second;
}
void grammar::destroy_rules()
{
     for(auto rit: grammar::rules)
     {
         rule *r = rit.second;
         delete r;
     }
    rules.clear();
}

term::term(rule *r, term_properties a_)
{
    a = a_;
    object.r = r;
    _type = term::type::r;
}

term::term(vxio::type::T a_sem, term_properties a_)
{
    a = a_;
    object.sem = a_sem;
    _type = term::type::s;
}

term::term(mnemonic a_code, term_properties a_)
{
    a = a_;
    object.m = a_code;
    _type = term::type::m;
}

term::term(const std::string &a_lexem)
{
    _type = term::type::m;
    object.m = lexem::from_str(a_lexem);
}

term::term(term &&_t)
noexcept
{
    //     logdebugfn << ":" << Ends;
    using std::swap;
    swap(object, _t.object);
    _type = _t._type;
    swap(a, _t.a);
}

term::term(const term &_t)
{
    object = _t.object;
    _type  = _t._type;
    a      = _t.a;
}

term &term::operator=(term &&_t) noexcept
{
    using std::swap;
    swap(object, _t.object);
    _type = _t._type;
    swap(a, _t.a);
    return *this;
}

term &term::operator=(const term &_t)
{
    object = _t.object;
    _type  = _t._type;
    a      = _t.a;
    return *this;
}

bool term::operator==(const term &t) const
{
    if(_type != t._type)
        return false;
    
    switch(_type)
    {
        case type::m:return object.m == t.object.m;
        case type::r:return object.r == t.object.r;
        case type::s:return (object.sem & t.object.sem) != 0;
        case type::nil:return false;
    }
    return false;
}

bool term::operator==(const token_data &t) const
{
    switch(_type)
    {
        case type::m:return object.m == t.c;
        case type::s:return (object.sem & t.s) != 0;
        case type::nil:
        default:return false;
    }
    return false;
}

bool term::operator!=(const token_data &t) const
{
    switch(_type)
    {
        case type::m:return object.m != t.c;
        case type::s:return (object.sem & t.s) == 0;
        case type::nil:
        default:return true;
    }
    return true;
}

term::~term()
{
}

std::string term::operator()() const
{
    iostr str;
    str << a();
    
    //    std::map<term::type, std::string> _{
    //        {term::type::rule, color::attribute(color::HRed)},
    //        {term::type::s,  color::attribute(color::HGreen)},
    //        {term::type::m, color::attribute(color::HBlue)}
    //    };
    //
    //    str << _[_type];
    switch(_type)
    {
        case term::type::m:
        {
            token_data tok = token_data()[object.m];
            str << tok.text(); // String (TEXT)
        }
            break;
        case term::type::r:
            // Can't happen but we never know: (nullptr)
            if(object.r)
                str << object.r->_id;
            break;
        case term::type::s:str << vxio::type::name(object.sem);
            break;
        default:str << "nil";
            break;
    }
    
    return str();
}

rule::rule(const std::string &a_id)
{
    _id = a_id;
    sequences.push_back({});
    seq = sequences.begin();
}

rule::~rule()
{
    sequences.clear();
    _id.clear();
}

rule &rule::new_sequence()
{
    sequences.push_back(term_seq());
    seq = --sequences.end();
    a.Reset();
    return *this;
}

rule &rule::operator|(rule *_r)
{
    term t = term(_r);
    t.a = a;
    a.Reset();
    *seq << t;
    return *this;
}

rule &rule::operator|(vxio::type::T _t)
{
    term t = term(_t);
    t.a = a;
    a.Reset();
    *seq << t;
    return *this;
}

rule &rule::operator|(mnemonic _t)
{
    term t = term(_t);
    t.a = a;
    a.Reset();
    *seq << t;
    return *this;
}

term term_seq::next(term::const_iterator &it) const
{
    if(it != terms.end())
        ++it;
    return *it;
}

term_seq &term_seq::operator<<(term a_t)
{
    terms.push_back(a_t);
    return *this;
}

term_seq &term_seq::operator<<(vxio::type::T a_t)
{
    terms.emplace_back(a_t);
    return *this;
}

term_seq &term_seq::operator<<(mnemonic a_t)
{
    terms.emplace_back(a_t);
    return *this;
}

term_seq &term_seq::operator<<(rule *a_t)
{
    terms.emplace_back(a_t);
    return *this;
}

std::string term_properties::operator()()
{
    iostr str;
    if(Z)
        str << "*";
    if(R)
        str << "+";
    if(L)
        str << "?";
    if(X)
        str << "#";
    return str();
}

}

