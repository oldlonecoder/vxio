//
// Created by oldlonecoder on 21-12-10.
//

#include <vxio/interpret/compiler/expression.h>
#include <vxio/util/logger.h>
#include <vxio/util/geometry.h>

namespace vxio
{

rem::code expression::parse(context ctx_, std::function<expect<>(context&)> lamda_fn)
{
    
    _ctx = std::move(ctx_);
    
    std::cout << "debug (check _ctx): " << _ctx.describe() << "\n";
    
    return rem::code::implement;
}

xio *expression::create_xio(token_data *token_)
{
    
    switch(token_->t)
    {
        //...
        default:return new xio(_ctx.blk, token_, nullptr);
    }
    return nullptr;
}



expression::~expression()
{

}

}