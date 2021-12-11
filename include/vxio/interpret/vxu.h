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
    vxu() = default;
    ~vxu() override;
    
    vxu(std::string fname_);
    
    alu jsr() override;
    rem::code load_code();
    
};
}
//#endif //VXIO_VXU_H
