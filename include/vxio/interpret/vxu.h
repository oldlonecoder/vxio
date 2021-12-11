//
// Created by oldlonecoder on 21-12-10.
//

//#ifndef VXIO_VXU_H
//#define VXIO_VXU_H

#pragma once
#include <vxio/interpret/bloc.h>
#include <vxio/lexer/lexer.h>

namespace vxio
{
class vxu : public bloc
{
    std::string  _fname; ///< .../filename.vxu
    std::string  _id;    ///< .../{filename}.vxu
    friend class interpret;
    token_data::collection _tokens;
    lexer              _lexer;
public:
    struct VXIO_API_DLL config
    {
        std::string  filename; ///< .../filename.vxu
        const char*  source;
        token_data*  tokens;
        //...
    };
    vxu() = default;
    ~vxu() override;
    
    vxu(xio* parent_);
    
    alu jsr() override;
    rem::code load_code();
    vxu::config& config_data() { return _config; }
protected:
    vxu::config _config;
    
    
};
}
//#endif //VXIO_VXU_H
