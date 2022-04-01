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

class VXIO_API schema
{
    class VXIO_API table
    {
        friend class schema;
    public:
        using collection = std::vector<table>;
        //...
        class VXIO_API field
        {
            friend class table;
            friend class schema;
        public:
            struct type
            {
                uint8_t Integer : 1;
                uint8_t Varchar : 1;
                uint8_t Binary : 1;
                uint8_t DateTime : 1;
            };
            
            struct
            {
                uint8_t i : 1; ///< Index
                uint8_t k : 1; ///< Key
                uint8_t p : 1; ///< Primary
                uint8_t f : 1; ///< Foreign
                uint8_t c : 4; ///< unused/reserved;
            };
            

            using collection = std::vector<field>;

            field() = default;
            ~field();

            field(const std::string& name_, const std::string& parent_table_name_, schema::table::field::type ftype_, uint8_t index_bits = 0);
            field(const std::string& name_, const std::string& parent_table_name_, schema::table::field::type ftype_, const std::string& fkey_name_);

        };
        
    private:
        field::collection _fields;
        int sz;
        uint8_t index_bits = 0;
        field::type _type = { 1,0,0,0 };

    };
    
    table::collection _tables;
    
};

}


