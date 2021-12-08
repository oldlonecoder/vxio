//
// Created by oldlonecoder on 21-12-08.
//

#include <vxio/interpret/bloc.h>
#include <vxio/util/logger.h>


namespace vxio
{

bloc::bloc(xio* pxio, token_data* ptoken, alu* a):xio(pxio,ptoken,a)
{
    logger::debug() <<  " bloc/scope newly created; ";
}


/*!
    @brief Clears children and instances created in this bloc.

    @note Must be called prior the destructor of tis instance.
*/

void bloc::clear()
{
    std::cout << __FUNCTION__ << ":\n";
    std::cout << __FUNCTION__ << " variables:\n";
    for (auto* x : variables)
    {
        if (!x->mem.xshared)
        {
            std::cout << __FUNCTION__ << " variables delete:'" << x->t0->text() << "'\n";
            delete x;
        }
        else
            std::cout << __FUNCTION__ << " variable '" << x->t0->text() << "' is shared, not delete here\n";
    }
    variables.clear();

    std::cout << __FUNCTION__ << " instructions:\n";
    for (auto* i : instructions)
    {
        if (!i->mem.xshared)
            delete i;
    }
    instructions.clear();

    std::cout << __FUNCTION__ << " functions:\n";
    for (auto* f : functions)
        delete f;
    functions.clear();

    //if (mem.xshared) delete acc; // occures in base xio class destructor;
}

bloc::~bloc()
{

}



bloc& bloc::operator=(const bloc& rsh)
{
    return *this;
}

alu bloc::jsr()
{
    if(!acc)
    {
        logger::warning() <<  " internal: acc is null - creating it.";
        acc = new alu(0.0f);
        mem.own = 1;
    }
    
    for(auto *i : instructions)
    {
        *acc = i->jsr();
        if(i->mem.leave)
        {
            i->mem.leave = 0;
            return *acc;
        }
    }
    return *acc;
}


xio* bloc::query_variable(const std::string& id)
{
    bloc* b = this;
    while (b)
    {
        if (xio* x; (x = b->query_local_variable(id))) return x;
        b = b->parent<bloc>();
    }

    return nullptr;
}

xio* bloc::query_local_variable(const std::string& id)
{
    //xio* x = nullptr;
    for (auto* x : variables)
        if (x->t0->text() == id)
            return x;
    return nullptr;
}

xio* bloc::query_function(const std::string& id)
{
    return nullptr;
}

xio* bloc::query_object(const std::string& id)
{
    return nullptr;
}

/*
* // @note
    ...continue search 'a' from this scope bloc.
    {
        number b = 0;
        //search 'a' from this scope bloc.
        //number a = 0.01;
        {
            string a = "allo"; ///<--- warning shadow parent scope declaration/definition.
            .::a = 0.10101010f;
        }
    }
*/
xio* bloc::create_local_variable(token_data* token, std::pair<bool, vxio::type::T> attr)
{
    xio* v = query_local_variable(token->text());
    if (v)
        return create_local_reference_variable(token, v);
    logger::debug() << " => " << token->details();
    v = new xio(this, token);
    v->t0->s |= attr.second|vxio::type::number_t;
    // pour static storage, on es pas encore rendu la...
    variables.push_back(v);
    return v;
}

xio* bloc::create_local_reference_variable(token_data* token, xio* ref)
{
    xio* x = new xio(this, token, ref->acc);
    x->mem.own = 0;
    logger::debug() <<  " => " << token->details();
    //variables.push_back(x);
    return x;
}


xio* bloc::create_local_instruction(xio* x)
{
    instructions.push_back(x);
    return x; // ?
}

xio* bloc::create_local_object(token_data* token)
{
    return nullptr;
}



void bloc::dump_variables()
{
    logger::output() << "\n bloc/scope variables:\n----------------------";
    for (auto v : variables)
    {
        logger::output() << color::White << "'"
            << color::Fuchsia << v->attribute()
            << color::White << "' => '"
            << color::Aquamarine3 << v->t0->semantic_types()
            << color::White << " = "
            << color::Lime << v->unit()();
    }
    logger::output() << "\n----------------------";
}



xio * bloc::entry()
{
    if(instructions.empty())
    {
        logger::warning() << " bloc stack contains no instructions...";
        return nullptr;
    }
    
    return *instructions.begin();
}

}
