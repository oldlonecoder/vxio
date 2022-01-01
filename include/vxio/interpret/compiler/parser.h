//
// Created by oldlonecoder on 21-12-10.
//

//#ifndef VXIO_PARSERBASE_H
//#define VXIO_PARSERBASE_H
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


#include <map>
#include <vxio/interpret/interpret.h>

namespace vxio{


class parser
{
    using parsers_map = std::map<std::string, parser*>;
    
public:
    struct context_t
    {
        bloc* blk = nullptr;
        token_data::cache   tokens_cache;
        token_data::pointer token_ptr;
        token_data::iterator   cursor,head;
        token_data::collection* tokens = nullptr;
        const rule* r = nullptr;
        
        context_t() = default;
        context_t(context_t&&) noexcept=default;
        context_t(const context_t&)=default;
        
        bool operator++();
        bool operator++(int);
        
        bool operator--();
        bool operator--(int);
        
        [[nodiscard]] bool end(token_data::iterator it) const;
        [[nodiscard]] bool end() const;
        
        parser::context_t& operator=(parser::context_t&&)noexcept=default;
        parser::context_t& operator=(const parser::context_t&)=default;
        void sync(const parser::context_t& rhs);
        token_data::pointer begin_cache();
        std::string status();
        std::string cache();
        inline void clear_cache() { tokens_cache.clear(); }
    };
    
protected:
    context_t context;
    token_data::collection* tokens = nullptr;
    
public:
    
    using assembler_fn = std::function<rem::code(parser::context_t&)>;
    
    
    parser()=default;
    parser& set_bloc(bloc* blk_);
    parser& set_tokens_stream(token_data::collection * tokens_);
    parser& set_assembler(parser::assembler_fn assembler);
    
    virtual ~parser() = default;
    virtual rem::code parse(const std::string& rule_id);
    virtual rem::code parse_rule(const rule* rule_);
    //virtual rem::code parse_sequence(const term_seq& seq);
    
protected:
    assembler_fn assembler_fnptr = nullptr; //
};

}

//#endif //VXIO_PARSERBASE_H
