#pragma once
#include <vxio/db/sqlite/sqlite3.h>
#include "vxio/util/logger.h"
#ifdef _WIN32
    #include <io.h> 
#else
#include <unistd.h>
#endif


namespace sql {

/**
 * @todo write docs
 */
class VXIO_API lite
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


