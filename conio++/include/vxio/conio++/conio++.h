#pragma once

#include <vxio/conio++/dll_config.h>

#include <vxio/util/geometry.h>
#include <vxio/util/rem.h>
#include <vxio/conio++/vdc.h>

#ifdef  _WIN32
#include <io.h>
#include <share.h>
#endif 
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>


namespace vxio::conio
{

struct CONIO_DLL ts
{	
	static vxio::size sz;

	static rem::code init();
	static rem::code get_wh();
	//...
	static rem::code put(const std::string& line);
    static rem::code put(char c);
    static void put_attr(const vch& attr);
    static void set_xy(const vxy& xy);
};

}
