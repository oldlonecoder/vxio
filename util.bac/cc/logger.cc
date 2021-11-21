//
// Created by oldlonecoder on 21-11-20.
//

#include <vxio/util/logger.h>
#include <fstream>


rem::list logger::_rems;




rem &logger::debug(const source::location &src)
{
    logger::_rems.emplace_back(rem::type::debug, src);
    return logger::_rems.back();
}
