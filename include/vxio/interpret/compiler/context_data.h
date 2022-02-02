//
// Created by oldlonecoder on 21-12-10.
//

/**********************************************************************************************************************************
 C opyright 2021 Serge Lussier (oldlonecoder), lussier.serge@gmail.com                                                              *
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

#include <map>
#include <vxio/interpret/bloc.h>
#include <stack>
#include <vxio/lexer/token_data.h>
#include <vxio/interpret/compiler/grammar.h>
#include <vxio/interpret/compiler/axt.h>


namespace vxio {
namespace compiler {

/**
 * @todo write docs
 */
struct context_data
{
    bloc* blk = nullptr;
    const grammar::rule* r  = nullptr;
    axt* expr_input         = nullptr;  ///< Arithmetic Expression Tree Input location.
    axt* expr_root          = nullptr;  ///< Arithmetic Expression Tree Root ( tree top/entry ).
    xio* instruction        = nullptr;  ///< Produced Instruction Object.

    token_data::cache   tokens_cache;
    token_data::pointer token_ptr;
    token_data::iterator   cursor,head;
    token_data::collection* tokens = nullptr;

    token_data  type_of;
    bool        static_storage  = false;

    xio::collection xio_cache;


    context_data() = default;
    context_data(context_data&&) noexcept;
    context_data(const context_data&);

    bool operator++();
    bool operator++(int);

    bool operator--();
    bool operator--(int);

    [[nodiscard]] bool end(token_data::iterator it) const;
    [[nodiscard]] bool end() const;

    context_data& operator=(context_data&&)noexcept;
    context_data& operator=(const context_data&);
    void sync(const context_data& rhs);
    token_data::pointer begin_cache();
    std::string status();
    std::string cache();
    inline void clear_cache() { tokens_cache.clear(); }
    static int push(const context_data& ctx);
    static int pop(context_data& ctx, bool synchronise = false);
    context_data& operator << (token_data::iterator it);
    static std::stack<context_data> stack;
    void restart_sequence();
};



}}


