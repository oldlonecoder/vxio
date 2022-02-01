// SPDX-FileCopyrightText: 2022 <copyright holder> <email>
// SPDX-License-Identifier: BSD-3-Clause

#include <vxio/interpret/compiler/context_data.h>


using namespace vxio::compiler;

std::stack<context_data> context_data::stack;



bool context_data::operator++()
{
    if(cursor == tokens->end())
        return false;
    ++cursor;
    return cursor == tokens->end();
}

bool context_data::operator++(int)
{
    if(cursor== tokens->end())
        return false;
    ++cursor;
    return cursor == tokens->end();
}

bool context_data::end(token_data::iterator it) const
{
    return it >= tokens->end();
}


bool context_data::end() const
{
    return cursor == tokens->end();
}



///
//{
//    cursor = rhs.cursor;
//...
//}

std::string context_data::cache()
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
bool context_data::operator--()
{
    if(token_ptr == tokens_cache.end()) return false;
    ++token_ptr;
    return token_ptr != tokens_cache.end();
}


bool context_data::operator--(int)
{
    if(token_ptr == tokens_cache.end()) return false;
    ++token_ptr;
    return token_ptr != tokens_cache.end();
}

vxio::token_data::pointer context_data::begin_cache()
{
    token_ptr = tokens_cache.begin();
    return token_ptr;
}


std::string context_data::status()
{
    iostr str;
    str << color::White << "context status on rule{%s%s%s}%s%s%s%s" ;
    std::string rl = r ? r->_id : "null";
    str  << color::Yellow << rl << color::White
    << rem::code_text(rem::code::endl) << color::White << cache() << rem::code_text(rem::code::endl)
    << (end(cursor) ? " end of stream ": cursor->mark());

    return str();
}

int context_data::push(const context_data& ctx)
{
    //logger::debug(src_location) << color::White  << " cursor {push} on " << color::Yellow << ctx.cursor->details(true);
    context_data::stack.push(ctx);
    return stack.size();
}

int context_data::pop(context_data& ctx, bool synchronise)
{
    if(stack.empty()) return 0;
    context_data to_sync = stack.top();
    //logger::debug(src_location) << color::White  << " cursor in stack on " << color::Yellow << to_sync.cursor->details(true);
    if(synchronise)
    {
        to_sync.cursor = ctx.cursor;
        to_sync.head = ctx.cursor;
        to_sync.instruction = ctx.instruction;
        //...
    }
    ctx = to_sync;
    stack.pop();
    return stack.size();
}



context_data::context_data(const context_data& rhs)
{
    blk = rhs.blk;
    tokens_cache = rhs.tokens_cache;
    token_ptr = rhs.token_ptr;
    type_of = rhs.type_of;
    static_storage = rhs.static_storage;
    cursor = rhs.cursor;
    head = rhs.head;
    tokens = rhs.tokens;
    r = rhs.r;
    instruction = rhs.instruction;
}

context_data::context_data(context_data && rhs) noexcept
{
    blk = rhs.blk;
    tokens_cache = std::move(rhs.tokens_cache);
    token_ptr = std::move(rhs.token_ptr);
    type_of = rhs.type_of;
    static_storage = rhs.static_storage;
    cursor = std::move(rhs.cursor);
    head = std::move(rhs.head);
    tokens = rhs.tokens;
    r = rhs.r;
    instruction = rhs.instruction;
}


context_data & context_data::operator=(const context_data& rhs)
{
    blk = rhs.blk;
    tokens_cache = rhs.tokens_cache;
    token_ptr = rhs.token_ptr;
    type_of = rhs.type_of;
    static_storage = rhs.static_storage;
    cursor = rhs.cursor;
    head = rhs.head;
    tokens = rhs.tokens;
    r = rhs.r;
    instruction = rhs.instruction;
    return *this;
}

context_data & context_data::operator=(context_data && rhs) noexcept
{
    blk = rhs.blk;
    tokens_cache = std::move(rhs.tokens_cache);
    token_ptr = std::move(rhs.token_ptr);
    type_of = rhs.type_of;
    static_storage = rhs.static_storage;
    cursor = std::move(rhs.cursor);
    head = std::move(rhs.head);
    tokens = rhs.tokens;
    r = rhs.r;
    instruction = rhs.instruction;
    return *this;
}


void context_data::sync(const context_data& rhs)
{
    cursor = rhs.cursor;
}

context_data& context_data::operator << (token_data::iterator it)
{
    tokens_cache.emplace_back(&(*it));
    return *this;
}

void context_data::restart_sequence()
{
    clear_cache();
    logger::debug(src_funcname) << "cursor on '" << color::Yellow
    << cursor->text() << color::White
    << "' to be back on head '" << color::Yellow <<
    head->text() << color::White << "'";
    cursor = head;
}
