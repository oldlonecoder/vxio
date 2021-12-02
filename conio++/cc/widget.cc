//
// Created by oldlonecoder on 21-11-28.
//

#include <vxio/conio++/widget.h>


namespace vxio::conio
{

widget::widget(widget *parent_):object(parent_)
{

}


widget::~widget()
{
    if(_dc->_own)
        delete _dc;
    
}


void widget::set_geometry(vxy xy_, size sz_)
{
    if(!_dc)
    {
        _dc = new vdc(this, sz_);
        
    }
}


}