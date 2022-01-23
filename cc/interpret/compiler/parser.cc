//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/compiler/parser.h>

/*!
 * @brief 2021-12-31 (!!) parser::context_t : cleanup; restore; rollback to previous state if no rule match; advance; finish rule.
 */
namespace vxio
{


std::stack<parser::context> parser::context::stack;

parser::assemblers_t parser::assemblers;


parser &parser::set_bloc(bloc *blk_)
{
    ctx.blk = blk_;
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
    ctx.tokens = tokens;
    ctx.head = tokens->begin();
    ctx.cursor = ctx.head;
    ctx.r = grammar()[rule_id];
    rem::code R = parse_rule(ctx.r);
    //...
    return R;

}

#define ContextElement color::Yellow << ctx.r->_id << color::White << "::'" << color::Yellow << (*elit)() << color::White << "' => token '" << color::Yellow << ctx.cursor->text() << color::White << "':"
#define Context color::White << "Current context : rule[" << color::Yellow << (ctx.r ? ctx.r->_id: "nullptr") << color::White << "] token cursor on '" << color::Yellow << ctx.cursor->text() << color::White << "'"


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

rem::code parser::parse_rule(const rule *rule_)
{

    return rem::code::implement;
}


/**
 * @brief iterates a sequence of elements.
 *
 * @author &copy;2022 Serge Lussier (lussier.serge@gmail.com); oldlonecoder
 * @return accepted or rejected.
 *
 * @note (fr) - Si la s&eacute;quence est optionnelle ( tous les &eacutesl&eacute;ments sont optionnels), il y aura un faux rem::code::accepted.
 */

rem::code parser::parse_sequence(const term_seq& sequence)
{
    if(sequence.terms.empty())
    {
        logger::error(src_funcname) << " Rejecting empty sequence.";
        return rem::code::empty;
    }
    ctx.clear_cache(); // New sequence then clear the tokens cache.

    auto elit = sequence.begin();
    logger::debug() << grammar().dump_sequence(sequence);

    do
    {

        ++elit;
    } while(!sequence.end(elit));


    return rem::code::implement;
}

bool parser::context::operator++()
{
    if(cursor == tokens->end())
        return false;
    ++cursor;
    return cursor == tokens->end();
}

bool parser::context::operator++(int)
{
    if(cursor== tokens->end())
        return false;
    ++cursor;
    return cursor == tokens->end();
}

bool parser::context::end(token_data::iterator it) const
{
    return it >= tokens->end();
}


bool parser::context::end() const
{
    return cursor == tokens->end();
}



///
//{
//    cursor = rhs.cursor;
    //...
//}

std::string parser::context::cache()
{
    using vxio::color;
    
    auto token = tokens_cache.begin();
    iostr str;
    str << color::White << "cache" << color::NavajoWhite3 << "{" << color::White;
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
    str << color::NavajoWhite3 << "};";
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
bool parser::context::operator--()
{
    if(token_ptr == tokens_cache.end()) return false;
    ++token_ptr;
    return token_ptr != tokens_cache.end();
}


bool parser::context::operator--(int)
{
    if(token_ptr == tokens_cache.end()) return false;
    ++token_ptr;
    return token_ptr != tokens_cache.end();
}

token_data::pointer parser::context::begin_cache()
{
    token_ptr = tokens_cache.begin();
    return token_ptr;
}


std::string parser::context::status()
{
    iostr str ="%scontext status on rule{%s%s%s}\n%s\n%s" ;
    std::string rl = r ? r->_id : "null";
    str << color::White << color::Yellow << rl << color::White << cache() << cursor->details(true);
    
    return str();
}

int parser::context::push(const parser::context& ctx)
{
    //logger::debug(src_location) << color::White  << " cursor {push} on " << color::Yellow << ctx.cursor->details(true);
    stack.push(ctx);
    return stack.size();
}

int parser::context::pop(parser::context& ctx, bool synchronise)
{
    if(stack.empty()) return 0;
    context to_sync = stack.top();
    //logger::debug(src_location) << color::White  << " cursor in stack on " << color::Yellow << to_sync.cursor->details(true);
    if(synchronise)
    {
        to_sync.cursor = ctx.cursor;
        to_sync.head = ctx.cursor;
        //...
    }
    ctx = to_sync;
    stack.pop();
    return stack.size();
}


rem::code parser::invoke_assembler()
{
    logger::debug(src_funcname) <<  rem::code::endl << ctx.status();

    if(assembler_fnptr)
        return assembler_fnptr(ctx);
    return rem::code::rejected;
}


}


vxio::parser::context::context(const vxio::parser::context& rhs)
{
    blk = rhs.blk;
    tokens_cache = rhs.tokens_cache;
    token_ptr = rhs.token_ptr;
    cursor = rhs.cursor;
    head = rhs.head;
    tokens = rhs.tokens;
    r = rhs.r;
}

vxio::parser::context::context(vxio::parser::context && rhs) noexcept
{
    blk = rhs.blk;
    tokens_cache = std::move(rhs.tokens_cache);
    token_ptr = std::move(rhs.token_ptr);
    cursor = std::move(rhs.cursor);
    head = std::move(rhs.head);
    tokens = rhs.tokens;
    r = rhs.r;
}


vxio::parser::context & vxio::parser::context::operator=(const parser::context& rhs)
{
    blk = rhs.blk;
    tokens_cache = rhs.tokens_cache;
    token_ptr = rhs.token_ptr;
    cursor = rhs.cursor;
    head = rhs.head;
    tokens = rhs.tokens;
    r = rhs.r;

    return *this;
}

vxio::parser::context & vxio::parser::context::operator=(parser::context && rhs) noexcept
{
    blk = rhs.blk;
    tokens_cache = std::move(rhs.tokens_cache);
    token_ptr = std::move(rhs.token_ptr);
    cursor = std::move(rhs.cursor);
    head = std::move(rhs.head);
    tokens = rhs.tokens;
    r = rhs.r;
    return *this;
}


void vxio::parser::context::sync(const parser::context& rhs)
{
    cursor = rhs.cursor;
}

vxio::parser::context& vxio::parser::context::operator << (token_data::iterator it)
{
    tokens_cache.emplace_back(&(*it));
    return *this;
}

void vxio::parser::context::restart_sequence()
{
    clear_cache();
    cursor = head;
}
