//
// Created by oldlonecoder on 21-12-10.
//

//#ifndef VXIO_PARSERBASE_H
//#define VXIO_PARSERBASE_H
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

#include <vxio/lexer/token_data.h>

#include <functional>
#include <utility>
#include <stack>
#include <map>
#include <vxio/util/expect.h>

namespace vxio {

/**
* @brief Arithmetic eXpression Tree
*
* @author &copy;2022 Serge Lussier (lussier.serge@gmail.com); oldlonecoder
*
* @note
*/
struct axt
{
    axt* op  = nullptr;
    axt* rhs = nullptr;
    axt* lhs = nullptr;

    token_data* t0 = nullptr;

    axt() = default;
    axt(token_data* token);
    ~axt() = default;

    using input_table_t = std::vector<std::pair<std::pair<type::T, type::T>,axt*(axt::*)(token_data*)>>;
    static input_table_t input_tbl;
    static std::vector<axt> axt_pool;

    // -------- Arithmetic binary tree INPUT: -----------------------------------
    #pragma region INPUT

    axt* input(token_data* token);
    axt* begin_tree(token_data* token);
    axt* close_par(token_data* token);

    axt* input_binary(token_data* token);
    axt* input_binary(axt* a);

    axt* input_leaf(token_data* token);
    axt* to_left(token_data* token);
    axt* to_right(token_data* token);
    axt* to_right_to_op(token_data* token);

    axt* to_left(axt* a);
    axt* to_right(axt* a);
    axt* to_right_to_op(axt* a);

    //axt::lr_input_fnptr associate(axt* a_lhs, axt* a_rhs);
    axt* lpar_input_binary(token_data* token);
    axt* input_rpar(token_data* token);
    axt* input_lpar(token_data* token);
    axt* rpar_input_postfix(token_data* token);
    axt* rpar_input_leaf(token_data* token);
    axt* rpar_rpar(token_data* token);

    #pragma endregion INPUT
    void* operator new(size_t sz, token_data* token);
};

}

