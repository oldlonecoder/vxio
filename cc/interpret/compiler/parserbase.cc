//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/compiler/parserbase.h>


namespace vxio
{

expect<xio *> parser_base::parse_rule(const rule *rule_)
{
    context::push(_ctx);
    
    auto seq = rule_->begin();
    while(!parse_sequence(*seq))
    {
        context::clear_xio_accumulator(_ctx.xio_accumulator);
        // ... cleanup de _ctx.xio_accumulator.
        seq++;
        if(rule_->end(seq))
        {
            context::pop(_ctx);
            return nullptr;
        }
    }
    expect<xio*> x = _ctx.instruction;
    context::pop(_ctx);
    return x;
}


expect<xio *> parser_base::parse_sequence(const term_seq &seq)
{
    xio* x = nullptr;
    for(const auto& trm : seq.terms)
    {
        if(trm.is_rule())
        {
            const rule* r = trm.object.r;
            auto x = parse_rule(r);
            if(!x)
                return nullptr;
            _ctx.xio_accumulator.push_back(*x);
            continue;
        }
        if(trm == *_ctx.c)
        {
            auto x = create_xio(*_ctx.c);
            if(!x)
                return nullptr;
        }
    }
    return x;
}


/**
 * @brief Really creates new default xio.
 * @param token_
 * @return pointer to the new xio
 */
expect<xio *> parser_base::create_xio(token_data &token_)
{
    _ctx.xio_accumulator.push_back(new xio(_ctx.blk, &token_));
    return _ctx.xio_accumulator.back();
}
void parser_base::push_instruction()
{

}

rem::code parser_base::parse(context ctx_)
{
    //...
    if(!_ctx._rule)
    {
        logger::error() << "declvar::parse (internal):" << " no such rule.";
        return rem::code::null_ptr;
    }

    std::pair<bool, type::T> attr{false, type::u64_t};
    auto x = parse_rule(_ctx._rule);

    return rem::code::rejected;
}
}
