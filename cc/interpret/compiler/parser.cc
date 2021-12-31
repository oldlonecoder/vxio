//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/compiler/parser.h>


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
    context.head = tokens->begin();
    
    auto R = parse_rule(context.r);
    
    // checkup;
    if(assembler_fnptr)
        assembler_fnptr(context);
    
    return rem::code::accepted;
}

expect<> parser::parse_rule(const rule *rule_)
{
    context.cursor = context.head;
    context_t saved_ctx = context;
    
    return logger::warning(src_long_funcname) << " implement";
}

expect<> parser::parse_sequence(const term_seq &seq)
{
    return logger::warning(src_long_funcname) << " implement";
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
}
