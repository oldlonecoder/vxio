#pragma once
#include <sqlite3.h>
#include "vxio/util/logger.h"
#include <unistd.h>

namespace sql {

/**
 * @todo write docs
 */
class lite
{
    sqlite3*    _connection = nullptr;
    std::string _dbname;

    static const char* ext;
    //...
public:
    lite();
    ~lite();
    
    rem::code open(const std::string& prefix_path, const std::string& name);
    rem::code close();

};

}


