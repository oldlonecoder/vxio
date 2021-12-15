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
    c     = ctx.c;
    last  = ctx.last;
    blk             = ctx.blk;
    xio_accumulator = ctx.xio_accumulator;
    i_tokens        = ctx.i_tokens;
}


context::context(bloc *bloc_, token_data::iterator first_, token_data::iterator last_):
start(first_), last(last_),blk(bloc_)
{

}


context::context(context &&ctx) noexcept:
    start(ctx.start),
c(ctx.c),
last(ctx.last),
blk(ctx.blk),
xio_accumulator(ctx.xio_accumulator),
i_tokens(ctx.i_tokens)
{

}


context &context::operator=(context &&ctx) noexcept
{
    start = ctx.start;
    c     = ctx.c;
    last  = ctx.last;
    blk             = ctx.blk;
    xio_accumulator = ctx.xio_accumulator;
    i_tokens        = ctx.i_tokens;
    return *this;
}


context &context::operator=(const context &ctx) noexcept
{
    start = ctx.start;
    c     = ctx.c;
    last  = ctx.last;
    blk             = ctx.blk;
    xio_accumulator = ctx.xio_accumulator;
    i_tokens        = ctx.i_tokens;
    return *this;
}


context::~context()
{
//    if(xio_accumulator)
//    {
        xio_accumulator.clear();
//        delete xio_accumulator;
//    }
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
    auto i = c;
    c++;
    return i;
}


token_data::iterator context::operator++()
{
    ++c;
    return c;
}
std::string context::describe()
{
    iostr str;
    // bloc id if vxu+ :
    if(!blk)
        exit(1);
    str = "bloc:%p '%s'";
    str << blk;
    
    if(vxu* xu = blk->to<vxu>())
    {
        str << (xu->id().empty() ? " context bloc: expected non-empty vxu::id!" : xu->id());
        if(xu->id().empty())
        {
            std::cout << str() << "\n  aborted with exit(1); !!";
            exit(1);
        }
    }
    else
        str << "scoped bloc";
    
    //...
    return str();
}
std::size_t context::clear_xio_accumulator(xio::collection &acc)
{
    for(auto* x : acc) delete x;
    acc.clear();
    return 0;
}

}