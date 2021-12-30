//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/compiler/parserbase.h>


namespace vxio
{

expect<> parser_base::parse_rule(const rule *rule_, std::function<expect<>(context&)> lamda_fn)
{
    context::push(_ctx);
    
    auto seq = rule_->begin();
    while(*parse_sequence(*seq,lamda_fn) != rem::code::accepted)
    {
        seq++;
        if(rule_->end(seq))
        {
            context::pop(_ctx);
            return rem::code::rejected;
        }
    }
    // sequence, then rule accepted.
    // invoke production here
    if(lamda_fn)
    {
        if(*lamda_fn(_ctx) != rem::code::accepted)
        {
            context::pop(_ctx);
            return rem::code::rejected;
        }
    }
    context::pop(_ctx);
    return rem::code::accepted;
}


expect<> parser_base::parse_sequence(const term_seq &seq, std::function<expect<>(context&)> lamda_fn)
{
    _ctx.i_tokens.clear();
    
    for(const auto& trm : seq.terms)
    {
        if(trm.is_rule())
        {
            repeat:
            if(*parse_rule(trm.object.r,lamda_fn) != rem::code::accepted)
            {
                if(trm.a.is_oneof()) continue;
            }
            if(trm.a.is_repeat())
                goto repeat;
            
            continue;
        }
        if(trm == *_ctx.c)
        {
            _ctx.push_token(_ctx.c);
            ++_ctx;
        }
    }
    return rem::code::accepted;
}



void parser_base::push_instruction()
{

}

rem::code parser_base::parse(context ctx_, std::function<expect<>(context&)> lamda_fn)
{
    //...
    
    _ctx = std::move(ctx_);
    if(!_ctx._rule)
    {
        logger::error() << "parser_base::parse (internal):" << " no such rule.";
        return rem::code::null_ptr;
    }

    std::pair<bool, type::T> attr{false, type::u64_t};
    auto x = parse_rule(_ctx._rule, lamda_fn);

    return rem::code::rejected;
}
}
