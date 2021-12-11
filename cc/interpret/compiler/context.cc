//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/compiler/context.h>

#include <vxio/util/rem.h>

namespace vxio
{


context::stack context::ctx_stack;

context::context(const context &ctx) noexcept
{
    start = ctx.start;
    finish = ctx.finish;
    last  = ctx.last;
    blk = ctx.blk;
    xios = ctx.xios;
    i_tokens = ctx.i_tokens;
}


context::context(bloc *bloc_, token_data::iterator first_, token_data::iterator last_):
start(first_), last(last_),blk(bloc_)
{

}


context::context(context &&ctx) noexcept:
start(ctx.start),
finish(ctx.finish),
last(ctx.last),
blk(ctx.blk),
xios(ctx.xios),
i_tokens(ctx.i_tokens)
{

}


context &context::operator=(context &&ctx) noexcept
{
    start = ctx.start;
    finish = ctx.finish;
    last  = ctx.last;
    blk = ctx.blk;
    xios = ctx.xios;
    i_tokens = ctx.i_tokens;
    return *this;
}


context &context::operator=(const context &ctx) noexcept
{
    start = ctx.start;
    finish = ctx.finish;
    last  = ctx.last;
    blk = ctx.blk;
    xios = ctx.xios;
    i_tokens = ctx.i_tokens;
    return *this;
}


context::~context()
{
    if(xios)
    {
        xios->clear();
        delete xios;
    }
    i_tokens.clear();
    while(!context::ctx_stack.empty()) context::ctx_stack.pop();
}


void context::push(const context& ctx)
{
    context::ctx_stack.push(ctx);
}

rem::code context::pop(context& ctx)
{
    if(context::ctx_stack.empty())
        return rem::code::empty;
    
    ctx = context::ctx_stack.top();
    context::ctx_stack.pop();
    return rem::code::ok;
}


token_data::iterator context::operator++(int)
{
    auto i = finish;
    finish++;
    return i;
}


token_data::iterator context::operator++()
{
    ++finish;
    return finish;
}


}