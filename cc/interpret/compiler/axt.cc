// SPDX-FileCopyrightText: 2022 <copyright holder> <email>
// SPDX-License-Identifier: BSD-3-Clause

#include <vxio/interpret/compiler/axt.h>


using namespace vxio;


std::vector<axt> axt::axt_pool; // A la fin de l'axt on ne fait qu'un axt::axt_pool.clear(); ! :)

axt::axt(token_data* token)
{
    t0 = token;
}

void* axt::operator new(size_t sz, token_data* token)
{
    axt::axt_pool.push_back({token});
    return &(*(--axt::axt_pool.end()));
}


axt::input_table_t axt::input_tbl =
{
    {{type::binary_t,     type::open_pair_t},  &axt::to_right},
    {{type::open_pair_t,  type::leaf_t},       &axt::to_left},
    {{type::open_pair_t,  type::prefix_t},     &axt::to_left},
    {{type::open_pair_t,  type::binary_t},     &axt::to_left},
    {{type::prefix_t,     type::open_pair_t},  &axt::to_right},
    {{type::close_pair_t, type::leaf_t},       &axt::rpar_input_leaf},
    {{type::close_pair_t, type::binary_t},     &axt::close_par},
    {{type::close_pair_t, type::postfix_t},    &axt::close_par},
  //{{type::close_pair_t, type::close_pair_t}, &axt::close_par},
    {{type::close_pair_t, type::close_pair_t}, &axt::rpar_rpar},
    {{type::prefix_t,     type::close_pair_t}, &axt::input_rpar},
    {{type::leaf_t,       type::close_pair_t}, &axt::input_rpar},
    {{type::leaf_t,       type::postfix_t},    &axt::to_right_to_op},
    {{type::leaf_t,       type::assign_t},     &axt::input_binary},
    {{type::leaf_t,       type::binary_t},     &axt::input_binary},
    {{type::postfix_t,    type::close_pair_t}, &axt::input_rpar},
    {{type::open_pair_t,  type::binary_t},     &axt::to_left},
    {{type::binary_t,     type::binary_t},     &axt::input_binary},
    {{type::binary_t,     type::leaf_t},       &axt::to_right},
    {{type::prefix_t,     type::binary_t},     &axt::input_binary},
    {{type::binary_t,     type::prefix_t},     &axt::to_right},
    {{type::prefix_t,     type::leaf_t},       &axt::to_right},
    {{type::prefix_t,     type::number_t},     &axt::to_right},
    {{type::sign_t,       type::id_t},         &axt::to_right},
    {{type::sign_t,       type::number_t},     &axt::to_right},
    {{type::sign_t,       type::leaf_t},       &axt::to_right},
    {{type::postfix_t,    type::binary_t},     &axt::input_binary},
    {{type::assign_t,     type::binary_t},    &axt::to_right},
    {{type::assign_t,     type::leaf_t},      &axt::to_right},
    {{type::assign_t,     type::prefix_t},    &axt::to_right},
    {{type::assign_t,     type::postfix_t},   &axt::to_right}

};


axt* axt::input(token_data* token)
{
    return nullptr;
}

axt* axt::begin_tree(token_data* token)
{
    return nullptr;
}
axt* axt::close_par(token_data* token)
{
    return nullptr;
}

axt* axt::input_binary(token_data* token)
{
    return nullptr;
}
axt* axt::input_binary(axt* a)
{
    return nullptr;
}

axt* axt::input_leaf(token_data* token)
{
    return nullptr;
}
axt* axt::to_left(token_data* token)
{
    return nullptr;
}

axt* axt::to_right(token_data* token)
{
    return nullptr;
}

axt* axt::to_right_to_op(token_data* token)
{
    return nullptr;
}

axt* axt::to_left(axt* a)
{
    return nullptr;
}

axt* axt::to_right(axt* a)
{
    return nullptr;
}

axt* axt::to_right_to_op(axt* a)
{
    return nullptr;
}

//axt::lr_input_fnptr associate(axt* a_lhs, axt* a_rhs);
axt* axt::lpar_input_binary(token_data* token)
{
    return nullptr;
}

axt* axt::input_rpar(token_data* token)
{
    return nullptr;
}

axt* axt::input_lpar(token_data* token)
{
    return nullptr;
}

axt* axt::rpar_input_postfix(token_data* token)
{
    return nullptr;
}

axt* axt::rpar_input_leaf(token_data* token)
{
    return nullptr;
}

axt* axt::rpar_rpar(token_data* token)
{
    return nullptr;
}



