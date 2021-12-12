//
// Created by oldlonecoder on 21-12-10.
//

//#ifndef VXIO_VXU_H
//#define VXIO_VXU_H
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
        //...
    };
    vxu() = default;
    ~vxu() override;
    
    vxu(xio* parent_);
    
    alu jsr() override;
    rem::code load_code();
    vxu::config& config_data() { return _config; }
    rem::code compile();
    std::string id() { return _id; }
protected:
    vxu::config _config;
    
    
};
}
//#endif //VXIO_VXU_H
