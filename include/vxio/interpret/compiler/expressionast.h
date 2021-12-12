//
// Created by oldlonecoder on 21-12-12.
//

//#ifndef VXIO_EXPRESSIONAST_H
//#define VXIO_EXPRESSIONAST_H

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
#include <vxio/interpret/compiler/context.h>
#include <utility>
namespace vxio
{
struct expression_ast
{
    context _ctx;
    
    struct node
    {
        token_data::iterator toki;
        node *lhs = nullptr;
        node *rhs = nullptr;
        node *op  = nullptr;
    };
    
    expression_ast() = default;
    ~expression_ast();
    
    expression_ast(context ctx_);
    
    
    using lr_fnptr = expression_ast::node *(expression_ast::*)(expression_ast::node *, token_data::iterator);
    
    using lr_pair = std::pair<vxio::type::T, vxio::type::T>;
    using lrf_pair = std::pair<expression_ast::lr_pair, expression_ast::lr_fnptr>;
    using lr_input_table = std::vector<expression_ast::lrf_pair>;
    
    void init_input_pairs();
    #pragma region input
    
    static expression_ast::lr_input_table input_fn_table;
    
    expression_ast::node *begin_input(token_data::iterator toki);
    expression_ast::node *close_tree();
    
    expression_ast::node *input(expression_ast::node *n, token_data::iterator toki);
    expression_ast::node *input_binary(expression_ast::node *n, token_data::iterator toki);
    expression_ast::node *input_leaf(expression_ast::node *n, token_data::iterator toki);
    expression_ast::node *set_left(expression_ast::node *n, token_data::iterator toki);
    expression_ast::node *set_right(expression_ast::node *n, token_data::iterator toki);
    expression_ast::node *set_right_to_op(expression_ast::node *n, token_data::iterator toki);
    //xio::inptr_fn_t associate(expression_ast::node* a_lhs, expression_ast::node* a_rhs);
    expression_ast::node *lpar_input_binary(expression_ast::node *n, token_data::iterator toki);
    expression_ast::node *input_rpar(expression_ast::node *n, token_data::iterator toki);
    expression_ast::node *input_lpar(expression_ast::node *n, token_data::iterator toki);
    expression_ast::node *close_par(expression_ast::node *n, token_data::iterator toki);
    expression_ast::node *rpar_input_postfix(expression_ast::node *n, token_data::iterator toki);
    expression_ast::node *rpar_input_leaf(expression_ast::node *n, token_data::iterator toki);
    expression_ast::node *rpar_rpar(expression_ast::node *n, token_data::iterator toki);
    #pragma endregion input
    
};

}
//#endif //VXIO_EXPRESSIONAST_H
