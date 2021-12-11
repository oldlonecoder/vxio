//
// Created by oldlonecoder on 21-12-08.
//
#pragma once
#include <vxio/interpret/xio.h>

namespace vxio
{
class VXIO_API_DLL bloc : public xio
{
protected:
    xio::collection variables; ///< reference and local variables.
    xio::collection instructions;
    xio::collection functions;
    xio::collection objects; ///< Sous r&eacute;serve ...
    // 1 + ::ABC::var_t // --- note : global namespace ==> root sibling vxu
    friend class vxu;
    friend class interpret;
public:

    bloc() :xio() {}
    bloc(xio* pxio, token_data* ptoken, alu* a);
    xio* entry();
    
    ~bloc() override;
    bloc& operator=(const bloc& rsh);
    alu jsr() override;

    xio* query_variable (const std::string& id);
    xio* query_local_variable(const std::string& id);
    xio* query_function(const std::string& id);
    xio* query_object(const std::string& id);
    
    xio* create_local_variable(token_data* token, std::pair<bool, vxio::type::T> attr = {false,vxio::type::any_t});
    xio* create_local_reference_variable(token_data* token, xio* ref);
    xio* create_local_instruction(xio* x);
    xio* create_local_object(token_data* token);

    virtual void clear();
    void dump_variables();


};

}