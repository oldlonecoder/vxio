

#include <vxio/interpret/vxu.h>

using namespace vxio;
using compiler::context_data;



expect<axt*> vxu::cc_expression(context_data& _cdata)
{
    return logger::fatal(src_funcname) << rem::code::implement;
}
