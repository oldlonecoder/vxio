//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/compiler/parser.h>

/*!
 * @brief 2021-12-31 (!!) parser::context_t : cleanup; restore; rollback to previous state if no rule match; advance; finish rule.
 */
namespace vxio
{


std::stack<parser::context_t> parser::context_t::stack;

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
    
    auto R = enter_rule(grammar()[rule_id]);
    return rem::code::accepted;
}

#define ContextElement color::Yellow << context.r->_id << color::White << "::" << color::Yellow << (*elit)() << color::White
#define Context color::Yellow << context.r->_id << color::White


/**
 * @brief Enters rule analyse.
 *
 * @ul
 *  @li Iterates the rule's list of alternative sequences.
 *  @li Once one of the sequences is accepted (matches the list of elements against the tokens):
 *  @ul
 *      @li Invokes the supplied "assembler" for the production from the cached tokens.
 *      @li Updates the parent context with the local context data
 *      @li return status as accepted to the parent context/recursion call up to the original invokation.s
 *
 * @author &copy;2022 Serge Lussier (lussier.serge@gmail.com); oldlonecoder
 * @return result code as rem::code value (reject or accepted);
 *
 * @note
 *
 */

rem::code parser::enter_rule(const rule *rule_)
{
    context_t::push(context);
    context.clear_cache();
    context.r = rule_;
    auto seqit = context.r->begin();
    rem::code code = rem::code::rejected;
    int i=1;
    size_t cnt = context.r->sequences.size();
    while(!context.r->end(seqit))
    {
        logger::debug(src_funcname) << grammar().dump_sequence(*seqit) << " - " << color::Yellow << cnt << color::White << '/' << color::LightPink4 << cnt;
        if((code = enter_sequence(*seqit)) == rem::code::accepted)
        {
            code = invoke_assembler();
            context_t::pop(context,code==rem::code::accepted);
            return code;
        }
        ++seqit;
    }
    logger::debug(src_funcname) << Context << " - rejected ";
    context_t::pop(context);
    return rem::code::rejected;
}


/**
 * @brief iterates a sequence of elements.
 *
 * @author &copy;2022 Serge Lussier (lussier.serge@gmail.com); oldlonecoder
 * @return accepted or rejected.
 *
 * @note (fr) - Si la s&eacute;quence est optionnelle ( tous les &eacutesl&eacute;ments sont optionnels), il y aura un faux rem::code::accepted.
 */

rem::code parser::enter_sequence(const term_seq& sequence)
{
    auto elit = sequence.begin();

    rem::code code = rem::code::rejected;
    context.clear_cache();
    logger::debug(src_funcname) << grammar().dump_sequence(sequence) << " : ";
    while(!sequence.end(elit))
    {
        logger::debug(src_funcname) << "element: '" << ContextElement << " <:> " << context.cursor->text();
        if(elit->is_rule())
        {
            logger::debug() << ContextElement << " is a rule.";
            repeat_after_me:
            if((code = enter_rule(elit->object.r)) == rem::code::accepted)
            {
                //...
                if(elit->a.is_oneof()) return code;
                if(elit->a.is_repeat()) goto repeat_after_me;
                if(elit->a.is_optional())
                {
                    ++elit;
                    continue;
                }
            }
            if(elit->a.is_optional())
            {
                ++elit;
                continue;
            }
            std::cout << (logger::debug() << ContextElement << " rejected").cc() << "\n";
            return rem::code::rejected; // more readable than "code";
        }
        // ----------------------------- iterate here -----------------------------
        if(*elit == *context.cursor)
        {
            logger::debug(src_funcname) << ContextElement << " matches token:" << rem::code::endl << context.cursor->mark();
            context.tokens_cache.push_back(&(*context.cursor));
            ++elit; ++context;
            if(sequence.end(elit))
            {
                logger::debug(src_funcname) << " sequence terminated and accpeted;";
                return rem::code::accepted;
            }
            logger::debug(src_funcname) << "next:"  << ContextElement;
            continue;
        }
        else
        {
            if(elit->a.is_optional())
            {
                logger::debug(src_funcname) << ContextElement << " optional.";
                ++elit;
                logger::debug(src_funcname) << "try next:"  << ContextElement;
                continue;
            }
        }
        logger::debug(src_funcname) << ContextElement << " Rejecting sequence.";
        return rem::code::rejected;
    }
    return code;
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



///
//{
//    cursor = rhs.cursor;
    //...
//}

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

int parser::context_t::push(const parser::context_t& ctx)
{
    stack.push(ctx);
    return stack.size();
}
int parser::context_t::pop(parser::context_t& ctx, bool synchronise)
{
    if(stack.empty()) return 0;
    context_t to_sync = stack.top();

    if(synchronise)
    {
        to_sync.cursor = ctx.cursor;
        // to_sync.instruction = ctx.instruction;
        //...
    }

    stack.pop();
    return stack.size();
}


rem::code parser::invoke_assembler()
{
    logger::debug(src_funcname) <<  rem::code::endl << context.status();

    if(assembler_fnptr)
        return assembler_fnptr(context);
    return rem::code::rejected;
}


}

