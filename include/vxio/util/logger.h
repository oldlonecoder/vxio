//
// Created by oldlonecoder on 21-11-20.
//

//#ifndef VXIO_FRAMEWORK_LOGGER_H
//#define VXIO_FRAMEWORK_LOGGER_H
#pragma once

#include <vxio/util/rem.h>
#include <functional>



class VXIO_API_DLL logger
{
	static rem::list _rems;

public:
	
	static void clear(std::function<void(rem&)> fn_);


	static rem& debug(const source::location& src = {});
	static rem& info(const source::location& src = {});
	static rem& fatal(const source::location& src = {});
	static rem& exception(const source::location& src = {});
	static rem& status(const source::location& src = {});
	static rem& output(const source::location& src = {});
	static rem& comment(const source::location& src = {});
	static rem& syntax(const source::location& src = {});
	static rem& error(const source::location& src = {});


};



//#endif //VXIO_FRAMEWORK_LOGGER_H
