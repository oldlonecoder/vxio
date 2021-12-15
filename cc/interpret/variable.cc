//
// Created by oldlonecoder on 21-12-15.
//

#include <vxio/interpret/variable.h>


namespace vxio
{

variable::~variable()
{

}
alu variable::jsr()
{
    return xio::jsr();
}


variable::variable(xio *a_parent_, token_data *a_token_, alu *a_alu_) : xio(a_parent_, a_token_, a_alu_)
{}


}
