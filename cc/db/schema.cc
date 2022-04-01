//
// Created by oldlonecoder on 22-03-09.
//

#include <vxio/db/schema.h>

namespace sql
{
    schema::table::field::~field()
    {
    }
    
    
    schema::table::field::field(const std::string& name_, const std::string& parent_table_name_, schema::table::field::type ftype_, uint8_t index_bits)
    {
    }
    schema::table::field::field(const std::string& name_, const std::string& parent_table_name_, schema::table::field::type ftype_, const std::string& fkey_name_)
    {
    }
}
