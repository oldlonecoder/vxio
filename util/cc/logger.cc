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

rem& logger::info(const source::location& src)
{
    logger::_rems.emplace_back(rem::type::info, src);
    return logger::_rems.back();
    // TODO: insérer une instruction return ici
}

rem& logger::fatal(const source::location& src)
{
    logger::_rems.emplace_back(rem::type::fatal, src);
    return logger::_rems.back();
    // TODO: insérer une instruction return ici
}

rem& logger::status(const source::location& src)
{
    logger::_rems.emplace_back(rem::type::status, src);
    return logger::_rems.back();
    // TODO: insérer une instruction return ici
}

rem& logger::output(const source::location& src)
{
    logger::_rems.emplace_back(rem::type::output, src);
    return logger::_rems.back();
    // TODO: insérer une instruction return ici
}

rem& logger::comment(const source::location& src)
{
    logger::_rems.emplace_back(rem::type::comment, src);
    return logger::_rems.back();
    // TODO: insérer une instruction return ici
}

rem& logger::syntax(const source::location& src)
{
    logger::_rems.emplace_back(rem::type::syntax, src);
    return logger::_rems.back();
    // TODO: insérer une instruction return ici
}

rem& logger::exception(const source::location& src)
{
    logger::_rems.emplace_back(rem::type::exception, src);
    return logger::_rems.back();
    // TODO: insérer une instruction return ici
}
