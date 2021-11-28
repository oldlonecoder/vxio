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

/*
 sqlitedb::sqlitedb(std::string&& a_dbname) noexcept
{
    utils::xstr str = std::move(a_dbname);
    _dbname = str();
    str << ".sqlitedb";

    std::cout << __FUNCTION__ << ": db file: [" << str() << "] \n\n";

}

sqlitedb::~sqlitedb()
{
    std::cout << __FUNCTION__ << '\n';
    close();
    _dbname.clear();
}

*!
 * @brief   Opens the sqlite3 database file.
 *
 * @note Do not let sqlite3_open auto-create the file automatically.
 * @return sqlitedb::code::ok.
 
sqlitedb::code sqlitedb::open()
{
    utils::xstr str = _dbname;
    str << ".sqlitedb";
#ifdef  _WIN32
    if (!PathFileExistsA(str().c_str()))
        return
        {
            (utils::notification::push(), " sqlite3 open db error(", _dbname, ") - no such database file.\n")
        };
#else
    int ok = access(str().c_str(),F_OK) == 0; // Just check if the db file exists.
    if(!ok)
    {
        return {(
            utils::notification::push(), "error openning database '", _dbname,"' - ",
            strerror(errno)
        )};
    }
#endif
    int res = sqlite3_open(str().c_str(), &_db);
    if (res != SQLITE_OK)
        return
        {
            (utils::notification::push(), " sqlitedb open db error(", res, ") - ", sqlite3_errmsg(_db))
        };

    std::cout << "(" << res << ")" << sqlite3_errmsg(_db) << "\n";
    
    return utils::notification::code::ok;
}

sqlitedb::code sqlitedb::close()
{
    if (_db)
    {
        utils::notification::push(), "db file [", _dbname, "] - closing. ";
        sqlite3_close(_db);
        _db = nullptr;
    }
    return utils::notification::code::ok;
}

sqlitedb::code sqlitedb::create()
{
    utils::xstr str = _dbname;
#ifdef  _WIN32
    if (PathFileExistsA(str().c_str()))
        return
        {
            (utils::notification::push(), " sqlite3 create db error(", _dbname, ") - database file already exists.\n")
        };
#endif
    int res = sqlite3_open(str().c_str(), &_db);
    if (res != SQLITE_OK)
        return
    {
            (utils::notification::push(), " sqlitedb open db error(", res, ") - ", sqlite3_errmsg(_db))
    };

    return utils::notification::code::ok;
}

}
 */
}
