#pragma once

#include <vxio/conio++/dll_config.h>

#include <vxio/util/geometry.h>
#include <vxio/util/rem.h>

#ifdef  _WIN32
#include <io.h>
#include <share.h>
#endif 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


namespace vxio::conio
{

struct CONIO_DLL ts
{	
	static vxio::size sz;

	static rem::code init();
	static rem::code get_wh();
	//...
	rem::code tput(const std::string& line);
};

}
