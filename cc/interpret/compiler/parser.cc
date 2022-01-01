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
    context.tokens = tokens;
    context.head = context.cursor = tokens->begin();
    
    auto R = parse_rule(grammar()[rule_id]);
    return rem::code::accepted;
}

#define ContextElement color::Yellow << local.r->_id << color::White << "::" << color::Yellow << (*elit)() << color::White
#define Context color::Yellow << local.r->_id << color::White

rem::code parser::parse_rule(const rule *rule_)
{
    auto saved_ctx = context;
    auto local = context;
    local.r = rule_;
    local.clear_cache();
    logger::debug(src_funcname) << ": entering rule '" << color::Yellow << local.r->_id << color::Reset << "':" << rem::code::endl << local.status();
    logger::debug() << " initiating rule's sequences list -> " << color::Yellow << local.r->sequences.size() << color::White << " alternative(s) :";
    auto seq_it = local.r->begin();
    int seq_index = 1;
    rem::code code = rem::code::rejected;
    while(!local.r->end(seq_it))
    {
        auto saved_cursor = local.cursor;
        logger::debug() << " initiating sequence elements list: " << color::Yellow << seq_index << color::White << "/" << color::Cyan3 << local.r->sequences.size()
        << rem::code::endl << grammar().dump_sequence(*seq_it);
        auto elit = seq_it->begin();
        while(!seq_it->end(elit))
        {
            logger::debug() << color::White << "element top loop " << ContextElement << ":";
            if(elit->is_rule())
            {
                logger::debug() << ContextElement << " is a rule, thus recurse parse_rule";
                code = parse_rule(elit->object.r);
                //context = saved_ctx;
                logger::debug() << ContextElement << " was " << code;
                if(code == rem::code::accepted)
                {

                    logger::debug() << ContextElement << " accepted.";
                    if(elit->a.is_repeat())
                    {
                        logger::debug() << "repeating " << ContextElement;
                        continue;
                    }
                    if(elit->a.is_oneof())
                    {
                        return rem::code::accepted;
                    }
                    ++elit;
                    continue;
                }
                optional_term:
                if(elit->a.is_optional())
                {
                    logger::debug() << ContextElement << "is optional so proceed to next element";
                    ++elit;
                    if(seq_it->end(elit))
                    {
                        logger::debug() << " no   more elements so "<< Context << " is rejected";
                        return rem::code::rejected;
                    }
                    continue;
                }

                ++elit;
                if(seq_it->end(elit))
                {
                    logger::debug() << " no more elements so "<< Context << " is rejected";
                    return rem::code::rejected;
                }
                continue;
            }
            if(*elit == *local.cursor)
            {
                local.tokens_cache.emplace_back(&(*local.cursor));
                logger::debug() << ContextElement <<  " matches token " << color::Yellow << local.cursor->text() << rem::code::endl << "token pushed on context cache." << rem::code::endl << local.cache();
                ++local;
                logger::debug() << Context << " next token: " << color::Yellow << local.cursor->text();
                if(elit->a.is_oneof())
                {
                    logger::debug() << ContextElement << " is a oneof then leaving as accepted";
                    if(assembler_fnptr)
                        return assembler_fnptr(context);
                    else
                        return rem::code::accepted;
                }
                ++elit;
                if(seq_it->end(elit))
                {
                    logger::debug() << Context << " Sequence #" << color::Cyan3 << seq_index << color::White << " complete match. leaving as rule was accepted";
                    saved_cursor = context.cursor;

                    context.cursor = saved_cursor;
                    if(assembler_fnptr)
                        return assembler_fnptr(context);
                    else
                        return rem::code::accepted;

                }
                logger::debug() << " We Are Here: " << ContextElement;
                continue;
            }
            logger::debug() << ContextElement << " no match, checking if element is optional:";
            if(elit->a.is_optional())
            {
                logger::debug() << ContextElement << "is optional so proceed to next element";
                ++elit;
                if(seq_it->end(elit))
                {
                    logger::debug() << " no   more elements so "<< Context << " is rejected";
                }
                else continue;
            }
            logger::debug() << ContextElement << " not optional, rejected.";
            break;
        }

        logger::debug() << "rule'" << color::Yellow << context.r->_id << color::Reset << " go to the next sequence:"<< rem::code::endl << context.cache();
        ++seq_it; ++seq_index;
        context.cursor = saved_cursor;
        context.clear_cache();
    }
    context = saved_ctx;
    return rem::code::rejected;
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
    //...
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
/**
 * @brief --> Advance the internal cached token iterator
 *
 * @author &copy;2022 Serge Lussier (lussier.serge@gmail.com); oldlonecoder
 * @return boolean, true if the iterator is not at the end of the cache, false otherwise.
 *
 * @note note.... lol.
 */
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
    iostr str ="%scontext status on rule{%s%s%s}\n%s\n%s" ;
    str << color::White << color::Yellow << r->_id << color::White << cache() << cursor->details(true);
    
    return str();
}

}
