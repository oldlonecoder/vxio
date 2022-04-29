//
// Created by oldlonecoder on 21-12-11.
//

//#ifndef VXIO_INTERPRET_H
//#define VXIO_INTERPRET_H

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
#include <vxio/interpret/vxu.h>

namespace vxio
{
    
/*!
  @note Restructure interpret with context - and - the parsers within the interpret class so move context_t to interpreet and remove the parser class files.
*/
    
class VXIO_API interpret : public bloc
{
    vxu* _main = nullptr;
    struct context_data
    {
        bloc* blk = nullptr;
        token_data::cache   tokens_cache;
        token_data::pointer token_ptr;
        token_data::iterator   cursor,head;
        token_data::collection* tokens = nullptr;
        const grammar::rule* r = nullptr;
        xio::collection xio_cache;  ///<  Sous reserve;
        xio* instruction = nullptr;


        context_data() = default;
        context_data(context_data&&) noexcept;
        context_data(const context_data&);
        
        bool operator++();
        bool operator++(int);
        
        bool operator--();
        bool operator--(int);
        
        [[nodiscard]] bool end(token_data::iterator it) const;
        [[nodiscard]] bool end() const;
        
        interpret::context_data& operator=(interpret::context_data&&)noexcept;
        interpret::context_data& operator=(const interpret::context_data&);
        void sync(const parser::context& rhs);
        token_data::pointer begin_cache();
        std::string status();
        std::string cache();
        inline void clear_cache() { tokens_cache.clear(); }
        static int push(const interpret::context_data& ctx);
        static int pop(interpret::context_data& ctx, bool synchronise = false);
        context_data& operator << (token_data::iterator it);
        static std::stack<interpret::context_data> stack;
        void restart_sequence();
    };
    
protected:
    context_data ctx;
public:
    interpret() = default;
    ~interpret() override;
    
    alu operator[](std::string src_);
    
    rem::code init();
    static interpret* instance();
    static const grammar::rule* get_rule(const std::string& name_);


};

}

//#endif //VXIO_INTERPRET_H
