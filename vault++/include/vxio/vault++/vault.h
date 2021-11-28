//
// Created by oldlonecoder on 21-11-28.
//

#pragma once

#include <vxio/vault++/dll_config.h>
#include <sqlite3.h>
#include <vxio/util/object.h>
#include <vxio/util/rem.h>

namespace vxio::db
{

class VXIO_VAULT_DLL vault : public object
{
    sqlite3* _db = nullptr;
    std::string _db_name;
    
public:
    vault() = default;
    
    vault(object* parent_, std::string  db_name_);
    ~vault();
    
    
    
};


}