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

}