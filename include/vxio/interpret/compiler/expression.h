//
// Created by oldlonecoder on 21-12-10.
//

//#ifndef VXIO_EXPRESSION_H
//#define VXIO_EXPRESSION_H

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
#include <vxio/interpret/compiler/parserbase.h>
#include <vxio/util/expect.h>
//#include <vxio/

namespace vxio
{
class expression : public parser_base
{
public:
    struct node
    {
        node* op = nullptr; ///< Operator (parent).
        node* ls = nullptr; ///< Left hand Side operand.
        node* rs = nullptr; ///< Right ahnd Side operand.
        token_data* token = nullptr;
        
        ~node();
        
        
        using result = expect<expression::node*>;
        using lr_pair = std::pair<type::T, type::T>;
        using lr_fnptr_pair = std::pair<lr_pair, expression::node::result(expression::node::*)(expression::node*)>;
        using lr_pair_table = std::vector<lr_fnptr_pair>;
        
        
        static lr_pair_table  lr_input_table;
        
        static expression::node::result begin_ast(token_data& token);
        static expression::node::result close_ast(token_data& token);
        
        expression::node::result input(token_data& token);
        expression::node::result input_leaf_binary_op(expression::node* n);
        expression::node::result input_id_open_pair(expression::node* n);
        expression::node::result input_leaf_open_pair(expression::node* n);
        expression::node::result input_prefix_open_pair(expression::node* n);
        expression::node::result input_fncall_open_pair(expression::node* n);
        
        expression::node::result op_input_binary_op(expression::node* n);
        expression::node::result input_binary_open_pair(expression::node* n);
        expression::node::result input_binary_leaf(expression::node* n);
        expression::node::result input_postfix_binary(expression::node* n);
        expression::node::result input_prefix_prefix(expression::node* n);
        expression::node::result set_left(node *n);
        expression::node::result set_right(node *n);
    };
public:
    expression():parser_base(){}
    ~expression() override;
    
    rem::code parse(context ctx_) override;
    xio* create_xio(token_data* token_);
};

}

//#endif //VXIO_EXPRESSION_H
