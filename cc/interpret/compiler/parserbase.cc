//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/compiler/parserbase.h>


namespace vxio
{

expect<> parser_base::parse_rule(const rule *rule_, std::function<expect<>(context&)> lamda_fn)
{
    logger::debug(src_long_funcname) << " enter rule '" << rule_->_id << "':\n";
    context::push(_ctx);
    expect<> R;
    auto seq = rule_->begin();
    while(*(R = parse_sequence(*seq,lamda_fn)) != rem::code::accepted)
    {
        seq++;
        if(rule_->end(seq))
        {
            context::pop(_ctx);
            return rem::code::rejected;
        }
    }
    if(*R != rem::code::accepted) return R;
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
    logger::debug(src_funcname) << _ctx.cached_tokens() << "\n";
    
    return rem::code::accepted;
}


expect<> parser_base::parse_sequence(const term_seq &seq, std::function<expect<>(context&)> lamda_fn)
{
    _ctx.i_tokens.clear();
    logger::debug(src_long_funcname) << " enter sequence: \n";
    auto it = seq.begin();
    while(!seq.end(it))
    {
        term trm = *it;
        logger::debug() << " " << _ctx._rule->_id << "::" << trm() << " :\n";
        if(trm.is_rule())
        {
            repeat:
            if(*parse_rule(it->object.r,lamda_fn) != rem::code::accepted)
            {
                if(it->a.is_oneof())
                {
                    ++it;
                    continue;
                }
            }
            if(it->a.is_repeat())
                goto repeat;
            ++it;
            continue;
        }
        if(*it == *_ctx.c)
        {
            _ctx.push_token(_ctx.c);
            ++_ctx;
        }
        else
        {
            logger::debug(src_long_funcname) << ":\n" << " no match in sequence in rule '" << _ctx._rule->_id << "': \n";
            logger::debug() << _ctx.c->details(true) << "\n checking term attributes:\n";
            if(!(it->a.is_oneof() || (it->a.is_optional())) )
            {
                logger::debug() << " token rejected, expected: " << (*it)();
                return rem::code::rejected;
            }
            if(it->a.is_oneof())
            {
                _ctx.push_token(_ctx.c);
                ++_ctx;
                return rem::code::accepted;
            }
        }
        ++it;
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
