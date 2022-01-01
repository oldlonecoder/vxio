//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/compiler/parser.h>

/*!
 * @brief 2021-12-31 (!!) parser::context_t : cleanup; restore; rollback to previous state if no rule match; advance; finish rule.
 */
namespace vxio
{

parser &parser::set_bloc(bloc *blk_)
{
    context.blk = blk_;
    return *this;
}

parser &parser::set_tokens_stream(token_data::collection *tokens_)
{
    tokens = tokens_;
    return *this;
}

parser &parser::set_assembler(parser::assembler_fn assembler)
{
    assembler_fnptr = std::move(assembler);
    return *this;
}

rem::code parser::parse(const std::string &rule_id)
{
    context.r = grammar()[rule_id];
    context.tokens = tokens;
    context.head = context.cursor = tokens->begin();
    
    auto R = parse_rule(context.r);
    
    return rem::code::accepted;
}

expect<> parser::parse_rule(const rule *rule_)
{
    context.tokens_cache.clear();
    context_t saved_ctx = context;
    context.head = context.cursor;
    context.r = rule_;
    logger::debug(src_funcname) << context.status();
    auto seqit = context.r->begin();
    while(!context.r->end(seqit))
    {
        
        auto e = parse_sequence(*seqit);
        if(!e || (*e != rem::code::accepted))
        {
            ++seqit;
            context = saved_ctx;
            continue;
        }
        if(*e == rem::code::accepted)
        {
            logger::debug(src_funcname) << " sequence accepted : " << context.status() << "\n";
            if(assembler_fnptr)
            {
                e = assembler_fnptr(context);
                if(!e || (*e != rem::code::accepted))
                {
                    context = saved_ctx;
                    return e;
                }
                context.tokens_cache.clear();
                return rem::code::accepted;
            }
        }
    }
    context = saved_ctx;
    return rem::code::rejected;
}

expect<> parser::parse_sequence(const term_seq &seq)
{
    auto elit = seq.begin();
    logger::debug(src_funcname) << ":\n" << "starting sequence element '" << color::Yellow << (*elit)() << color::White << "':\n";
    while(!seq.end(elit))
    {
        if(elit->is_rule())
        {
            repeat:
            logger::debug(src_funcname) << " element is rule '" <<  (*elit)() << "':\n";
            auto e = parse_rule(elit->object.r);
            logger::debug(src_funcname) << context.status() << "\n then rule element: '" << color::Yellow << (*elit)() << color::Reset;
            if(!e || (*e != rem::code::accepted))
            {
                if(elit->a.is_optional())
                {
                    ++elit;
                    continue;
                }
                return rem::code::rejected;
            }
            if(*e == rem::code::accepted)
            {
                context.tokens_cache.clear();
                if(elit->a.is_repeat())
                {
                    logger::debug(src_funcname) << "sequence element '" << (*elit)() << "' has repeat:";
                    goto repeat;
                }
                if(elit->a.is_oneof())
                {
                    logger::debug() << "element '" << (*elit)() << "' is a one_of then leaving.";
                    break;
                }
            }
        }
        if(*elit != *context.cursor)
        {
            logger::debug() << "no match between element '"  << color::Yellow << (*elit)() << color::Reset <<  "' and '" << context.cursor->text() << "'\n";
            if(elit->a.is_oneof())
            {
                ++elit;
                logger::debug() << " on next sequence element: '" << (*elit)() << "':";
                continue;
            }
            return rem::code::rejected;
        }
        logger::debug(src_funcname) << " element '" << (*elit)() << "' matches '" << context.cursor->text() << "'.";
        context.tokens_cache.push_back(&(*context.cursor));
        ++context;
        if(elit->a.is_oneof()) break;
        ++elit;
    }
    return rem::code::accepted;
}


bool parser::context_t::operator++()
{
    if(cursor == tokens->end())
        return false;
    ++cursor;
    return cursor == tokens->end();
}

bool parser::context_t::operator++(int)
{
    if(cursor == tokens->end())
        return false;
    ++cursor;
    return cursor == tokens->end();
}

bool parser::context_t::end(token_data::iterator it) const
{
    return it == tokens->end();
}


bool parser::context_t::end() const
{
    return cursor == tokens->end();
}
void parser::context_t::sync(const parser::context_t &rhs)
{
    cursor = rhs.cursor;
    
}
std::string parser::context_t::cache()
{
    using vxio::color;
    
    auto token = tokens_cache.begin();
    iostr str;
    str << color::Reset << "cache {";
    if(tokens_cache.empty())
    {
        str << color::Yellow <<  "Empty" << color::Reset << "};";
        return str();
    }
    while(token != tokens_cache.end())
    {
        str << color::Yellow <<  (*token)->text() << color::Reset;
        ++token;
        if(token != tokens_cache.end()) str += ' ';
    }
    str += "};";
    return str();
}
bool parser::context_t::operator--()
{
    if(token_ptr == tokens_cache.end()) return false;
    ++token_ptr;
    return token_ptr != tokens_cache.end();
}
bool parser::context_t::operator--(int)
{
    if(token_ptr == tokens_cache.end()) return false;
    ++token_ptr;
    return token_ptr != tokens_cache.end();
}

token_data::pointer parser::context_t::begin_cache()
{
    token_ptr = tokens_cache.begin();
    return token_ptr;
}
std::string parser::context_t::status()
{
    iostr str ="%scontext status:\nrule{%s%s%s}\n%s\n%s" ;
    str << color::White << color::Yellow << r->_id << color::White << cache() << cursor->details(true);
    
    return str();
}

}
