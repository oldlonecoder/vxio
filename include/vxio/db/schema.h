//
// Created by oldlonecoder on 22-03-09.
//

//#ifndef VXIO_SCHEMA_H
//#define VXIO_SCHEMA_H

/**********************************************************************************************************************************
    Copyright 2022 Serge Lussier (oldlonecoder), lussier.serge@gmail.com
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
#include <vxio/util/logger.h>


namespace sql
{

class schema
{
    class table
    {
        friend class schema;
    public:
        using collection = std::vector<table>;
        //...
        class field
        {
            friend class table;
            friend class schema;
            enum type
            {
                Integer,
                Varchar,
                Binary,
                DateTime
            };
            
            struct
            {
                uint8_t i : 1; ///< Index
                uint8_t k : 1; ///< Key
                uint8_t p : 1; ///< Primary
                uint8_t f : 1; ///< Foreign
            };
            int sz;
            using collection = std::vector<field>;
        };
        
    private:
        field::collection _fields;
        
    };
    
    table::collection _tables;
    
};

}


