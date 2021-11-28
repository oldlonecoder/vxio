//
// Created by oldlonecoder on 21-11-28.
//

#include <vxio/vault++/vault.h>

#include <utility>



namespace vxio::db
{

vault::vault(object *parent_, std::string db_name_):
object(parent_), _db_name(std::move(db_name_))
{

}


vault::~vault()
{
    //...
    _db_name.clear();
    if(_db)
    {
        sqlite3_close(_db);
        _db = nullptr;
    }
    //...
    
}


}
