//
// Created by oldlonecoder on 21-12-10.
//

//#ifndef VXIO_CONTEXT_H
//#define VXIO_CONTEXT_H

/**********************************************************************************************************************************
    Copyright 2021 Serge Lussier (oldlonecoder), lussier.serge@gmail.com
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
    THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*************************************************************************************************************************************/

#pragma once
#include "vxio/interpret/bloc.h"
#include <stack>
#include "vxio/interpret/vxu.h"

namespace vxio
{


class context
{
public:
    
    token_data::iterator start;
    token_data::iterator finish;
    token_data::iterator last;
    bloc*  blk = nullptr;
    xio* instruction = nullptr;
    xio::collection xios; ///< locally compiled xio's.
    
    std::vector<token_data::iterator> i_tokens; ///< (Sous r&eacute;serve) In expression parser for example, we would iterate
                                    ///< forward by filling in the local token iterators collection while the token
                                    ///< is a usable value in an arithmetic expression
    using stack = std::stack<context>;
    static context::stack ctx_stack;
    

    context()   = default;
    ~context();
    context(const context& ctx)noexcept;
    context(bloc* bloc_, token_data::iterator first_, token_data::iterator last_);
    context(context&& ctx) noexcept;
    
    context& operator=(context&& ctx)noexcept;
    context& operator=(const context& ctx)noexcept;
    
    static void push(const context& ctx);
    static rem::code pop(context& ctx);
    
    token_data::iterator operator++(int);
    token_data::iterator operator++();
    bool end() { return finish == last; }
    
    
    std::string describe();
    
};

}

//#endif //VXIO_CONTEXT_H
