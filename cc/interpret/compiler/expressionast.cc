//
// Created by oldlonecoder on 21-12-12.
//

#include <vxio/interpret/compiler/expressionast.h>



namespace vxio
{

expression_ast::lr_input_table expression_ast::input_fn_table =
{
        {{vxio::type::binary_t,     vxio::type::open_pair_t},  &expression_ast::set_right},
        {{vxio::type::binary_t,     vxio::type::open_pair_t},  &expression_ast::set_right},
        {{vxio::type::open_pair_t,  vxio::type::leaf_t},       &expression_ast::set_left},
        {{vxio::type::open_pair_t,  vxio::type::prefix_t},     &expression_ast::set_left},
        {{vxio::type::open_pair_t,  vxio::type::binary_t},     &expression_ast::set_left},
        {{vxio::type::prefix_t,     vxio::type::open_pair_t},  &expression_ast::set_right},
        {{vxio::type::close_pair_t, vxio::type::leaf_t},       &expression_ast::rpar_input_leaf},
        {{vxio::type::close_pair_t, vxio::type::binary_t},     &expression_ast::close_par},
        {{vxio::type::close_pair_t, vxio::type::postfix_t},    &expression_ast::close_par},
        {{vxio::type::close_pair_t, vxio::type::close_pair_t}, &expression_ast::close_par},
        {{vxio::type::close_pair_t, vxio::type::close_pair_t}, &expression_ast::rpar_rpar},

        {{vxio::type::prefix_t,     vxio::type::close_pair_t}, &expression_ast::input_rpar},
        {{vxio::type::leaf_t,       vxio::type::close_pair_t}, &expression_ast::input_rpar},
        {{vxio::type::leaf_t,       vxio::type::postfix_t},    &expression_ast::set_right_to_op},
        {{vxio::type::leaf_t,       vxio::type::assign_t},     &expression_ast::input_binary},
        {{vxio::type::postfix_t,    vxio::type::close_pair_t}, &expression_ast::input_rpar},
        {{vxio::type::open_pair_t,  vxio::type::binary_t},     &expression_ast::set_left},
        {{vxio::type::leaf_t,       vxio::type::binary_t},     &expression_ast::input_binary},
        {{vxio::type::binary_t,     vxio::type::binary_t},     &expression_ast::input_binary},
        {{vxio::type::binary_t,     vxio::type::leaf_t},       &expression_ast::set_right},
        {{vxio::type::prefix_t,     vxio::type::binary_t},     &expression_ast::input_binary},
        {{vxio::type::binary_t,     vxio::type::prefix_t},     &expression_ast::set_right},
        {{vxio::type::prefix_t,     vxio::type::leaf_t},       &expression_ast::set_right},
        {{vxio::type::prefix_t,     vxio::type::number_t},     &expression_ast::set_right},
        {{vxio::type::sign_t,       vxio::type::id_t},         &expression_ast::set_right},
        {{vxio::type::sign_t,       vxio::type::number_t},     &expression_ast::set_right},
        {{vxio::type::sign_t,      vxio::type::leaf_t},      &expression_ast::set_right},
        {{vxio::type::postfix_t, vxio::type::binary_t},  &expression_ast::input_binary},
        {{vxio::type::assign_t,  vxio::type::binary_t},  &expression_ast::set_right},
        {{vxio::type::assign_t,  vxio::type::leaf_t},    &expression_ast::set_right},
        {{vxio::type::assign_t,  vxio::type::prefix_t},  &expression_ast::set_right},
        {{vxio::type::assign_t,  vxio::type::postfix_t}, &expression_ast::set_right}
        
 
};

expression_ast::~expression_ast()
{

}

expression_ast::expression_ast(context ctx_)
{

}
expression_ast::node *expression_ast::begin_input(token_data::iterator toki)
{
    return nullptr;
}
expression_ast::node *expression_ast::close_tree()
{
    return nullptr;
}
expression_ast::node *expression_ast::input(expression_ast::node *n, token_data::iterator toki)
{
    return nullptr;
}
expression_ast::node *expression_ast::input_binary(expression_ast::node *n, token_data::iterator toki)
{
    return nullptr;
}
expression_ast::node *expression_ast::input_leaf(expression_ast::node *n, token_data::iterator toki)
{
    return nullptr;
}
expression_ast::node *expression_ast::set_left(expression_ast::node *n, token_data::iterator toki)
{
    return nullptr;
}
expression_ast::node *expression_ast::set_right(expression_ast::node *n, token_data::iterator toki)
{
    return nullptr;
}
expression_ast::node *expression_ast::set_right_to_op(expression_ast::node *n, token_data::iterator toki)
{
    return nullptr;
}
expression_ast::node *expression_ast::lpar_input_binary(expression_ast::node *n, token_data::iterator toki)
{
    return nullptr;
}
expression_ast::node *expression_ast::input_rpar(expression_ast::node *n, token_data::iterator toki)
{
    return nullptr;
}
expression_ast::node *expression_ast::input_lpar(expression_ast::node *n, token_data::iterator toki)
{
    return nullptr;
}
expression_ast::node *expression_ast::close_par(expression_ast::node *n, token_data::iterator toki)
{
    return nullptr;
}
expression_ast::node *expression_ast::rpar_input_postfix(expression_ast::node *n, token_data::iterator toki)
{
    return nullptr;
}
expression_ast::node *expression_ast::rpar_input_leaf(expression_ast::node *n, token_data::iterator toki)
{
    return nullptr;
}
expression_ast::node *expression_ast::rpar_rpar(expression_ast::node *n, token_data::iterator toki)
{
    return nullptr;
}

void expression_ast::init_input_pairs()
{
}

}
