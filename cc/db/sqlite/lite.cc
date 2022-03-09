#include <vxio/db/sqlite/lite.h>



namespace sql{

const char* lite::ext= "sqlite";


/**
 * @brief Constructor...
 *
 * @author &copy;2022 Serge Lussier (lussier.serge@gmail.com); oldlonecoder
 * @return
 *
 * @note
 */

lite::lite()
{

}

/**
 * @brief destroyer...
 *
 * @author &copy;2022 Serge Lussier (lussier.serge@gmail.com); oldlonecoder
 * @return
 *
 * @note
 */

lite::~lite()
{

}


/**
 * @brief
 *
 * @author &copy;2022 Serge Lussier (lussier.serge@gmail.com); oldlonecoder
 * @return
 *
 * @note
 */
rem::code lite::open(const std::string& prefix_path, const std::string& name)
{
    iostr str = "%s/%s.%s";
    str << prefix_path << name << lite::ext;

    logger::debug(src_funcname) << " full filename:" << str() << rem::code::endl;

    return rem::code::implement;
}


/**
 * @brief
 *
 * @author &copy;2022 Serge Lussier (lussier.serge@gmail.com); oldlonecoder
 * @return
 *
 * @note
 */
rem::code lite::close()
{
    return rem::code::implement;
}



}



