//
// Created by oldlonecoder on 21-11-20.
//

#include <vxio/util/logger.h>
#include <fstream>


rem::list logger::_rems;




void logger::clear(std::function<void(rem&)> fn_)
{
    // threads : only one would use the rems list.
    // Once cleared, its cleared. dot.
    // 
    // logger::_rems_guard.lock();
    if (logger::_rems.empty())
    {
        logger::_rems.clear();
        // logger::_rems_guard.unlock();
        return;
    }

    for (auto& e : logger::_rems)  if (fn_) fn_(e);
    // logger::_rems_guard.unlock();
}


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
