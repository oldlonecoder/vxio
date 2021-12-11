//
// Created by slussier on 19-08-25.
//
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
#include <functional>
#include <utility>
#include <stack>
#include <map>




#include <vxio/interpret/alu.h>
#include <vxio/lexer/token_data.h>


namespace vxio {

/*!
    @brief xio : eXecutable Instruction Object.
    Where the beach is its grain of sand, the Interpreter is its grain of xio. (FUCK! LOL!!!!)

    @author &copy;2018, Serge Lussier, (lussier.serge at gmail.com)
    if static i8 a=0.4*5.6 * 4ac(8+3 - 5/4)
        fncall();
    else
    {
        fn2call();
        return rtrem::ok;
    }
*/
class VXIO_API_DLL xio {
public:
        
    using collection = std::vector<xio*>;

protected:

    xio* op = nullptr; ///< Parent Operator
    xio* lhs = nullptr; ///< Left Hand Side Operand.
    xio* rhs = nullptr; ///< Right Hand Size Operand
        
    token_data* t0 = nullptr; ///< Token informations.
        
    alu* acc = nullptr; ///< Arithmetic Logical Unit, or Accumulator, or whatever!


    struct storage_attr {
        uint8_t lvalue : 1; // 0 = rvalue non-assignable (such as const,leaf). 1= left value; assignable.
        uint8_t own : 1; // 1 = this xio owns its acc storage - so will delete it in the destructor.
        uint8_t leave : 1; // 1 = leave flag because this xio is a return statement.
        uint8_t sstatic : 1; // 1 = static storage.
        uint8_t xshared : 1; // 1 = cannot be deleted in the parent's destructor.
        // ...
    }mem = { 0,0,0,0,0 }; ///< Storage types of this xio.
    friend class var;
    friend class bloc;
    // friend class compiler;
    // friend class interpreter_t;


public:

    xio();
    explicit xio(xio* a_parent);
    xio(xio* a_parent, token_data* a_token, alu* a_alu = nullptr);

    virtual ~xio();

    xio& operator =(const xio&) = delete;
    xio& operator =(xio&&) = delete;

    using expected = rem::code;

    using maker = std::function<xio*(token_data*)>;

    alu unit() { return (acc ? *acc : alu(0.0f)); }

    xio* tree_input(token_data* a_token, xio::maker a_allocator = nullptr);
    static xio* begin_expr(token_data* a_token, xio::maker invoke_maker);
    virtual alu jsr(); // "Jump Subroutine" :)


    virtual std::string type_name();

    xio* oper() { return op; }
    token_data* token() { return t0; }
    xio* tree_close();
    xio* tree_root(bool skip_syntax=false);

    template<class T> T* to() { return dynamic_cast<T*>(this); }
    static void illegal_rvalue(xio* x, token_data* token);


    void detach();
    void remove_child(xio* c);
private:

#pragma region Assoc
    // First : declare associative pair:
    using associative_type = std::pair<vxio::type::T, vxio::type::T>;
    using inptr_fn_t = xio*(xio::*)(xio*);        ///< callable xio tree input function ptr
    using associated_method = std::pair < xio::associative_type, xio::inptr_fn_t>;

    using input_table_t = std::vector<xio::associated_method>;
    static xio::input_table_t tree_input_assoc_table;

#pragma endregion Assoc

protected:

    using xio_opfn_t = alu(xio::*)();
    using xio_opfn_table_t = std::map<mnemonic, xio_opfn_t>;


    xio_opfn_t xio_fn = nullptr;

    static xio* get_global_root();
    virtual rem::code instanciate_from(xio* bcd);

    template<typename T> T* parent()
    {
        xio* x = _parent;
        while (x)
        {
            if (dynamic_cast<T*>(x)) return dynamic_cast<T*>(x);
            x = x->_parent;
        }
        return nullptr;
    }
private:

    static xio* _global_root_bloc;
    static std::stack<xio*> pars;
    static std::stack<xio*> indexes;
    static std::stack<xio*> curls;
    static void push_lpar(xio* lpar);
    static xio* pop_lpar();

    /*
        static void push_index(xio* index);
        static xio* pop_index();
        static void push_curly(xio* curly);
        static xio* pop_curly();
    */

    // -------- Arithmetic binary tree INPUT: -----------------------------------
#pragma region INPUT
    xio* tree_input_binary(xio* x);
    //xio* tree_input_binary(token_data* token, xio::maker xio_maker);
    
    xio* tree_input_leaf(xio* x);
    //xio* tree_input_leaf(token_data* token, xio::maker xio_maker);
    
    xio* tree_set_left(xio* x);
    //xio* tree_set_left(token_data* token, xio::maker xio_maker);
    
    xio* tree_set_right(xio* x);
    
    xio* tree_set_right_to_op(xio* x);

    xio::inptr_fn_t associate(xio* a_lhs, xio* a_rhs);

    xio* tree_lpar_input_binary(xio* x);

    xio* tree_input_rpar(xio*);
    xio* tree_input_lpar(xio*);
    xio* tree_close_par(xio*);
    xio* tree_rpar_input_postfix(xio*);
    xio* tree_rpar_input_leaf(xio*);
    xio* tree_rpar_rpar(xio*);

#pragma endregion INPUT
    // -------- Arithmetic binary tree: -----------------------------------


    static xio_opfn_table_t xio_operators_table;
    // ----------- Primary operators ----------------------------
#pragma region ARITMETIC
    virtual alu  lshift();
    virtual alu  radical();
    virtual alu  exponent();
    virtual alu  rshift();
    virtual alu  decr();
    virtual alu  incr();
    virtual alu  assignadd();
    virtual alu  assignsub();
    virtual alu  assignmul();
    virtual alu  assigndiv();
    virtual alu  assignmod();
    virtual alu  assignand();
    virtual alu  assignor();
    virtual alu  assignxor();
    virtual alu  assignx1();
    virtual alu  assignlshift();
    virtual alu  assignrshift();
    virtual alu  leq();
    virtual alu  geq();
    virtual alu  eq();
    virtual alu  neq();
    virtual alu  add();
    virtual alu  sub();
    virtual alu  mul();
    virtual alu  modulo();
    virtual alu  lt();
    virtual alu  gt();
    virtual alu  assign();
    virtual alu  binand();
    virtual alu  binor();
    virtual alu  bitxor();
    virtual alu  x1();
    virtual alu  x2();
    virtual alu  bitnot();
    virtual alu  booland();
    virtual alu  boolor();
    virtual alu  division();
    virtual alu  factorial();
    virtual alu  positive();
    virtual alu  negative();
    virtual alu  kpi();
    virtual alu  kcos();
    virtual alu  kacos();
    virtual alu  ktan();
    virtual alu  katan();
    virtual alu  ksin();
    virtual alu  kasin();
    virtual alu  knumber();
    virtual alu  ku8();
    virtual alu  ku16();
    virtual alu  ku32();
    virtual alu  ku64();
    virtual alu  ki8();
    virtual alu  ki16();
    virtual alu  ki32();
    virtual alu  ki64();
    virtual alu  kreal();
    virtual alu  kstring();

#pragma endregion ARITHMETICS

#pragma region TRIGRAPH
public:
    static void trace_tree_start(iostr& a_out, const iostr& Title);
    static void trace_node(xio* A, iostr& a_out);
    static void trace_null_node(xio*, int, iostr&);
    static void trace_tree(xio* a_root, iostr& a_out);
    static void trace_tree_close(iostr& a_out);
    void tree_attr(iostr& a_out);

    std::string attribute();
//    static std::string trace_connect_binary_operands(xio* x);
//    static std::string trace_connect_binary_operands2(xio* x);

#pragma endregion TRIGRAPH


protected:

    collection _children;
    xio* _parent = nullptr;
    xio* query_child(xio* cThis);

};
}
