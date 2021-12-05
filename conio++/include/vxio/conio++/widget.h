//
// Created by oldlonecoder on 21-11-28.
//

#pragma once
#include <vxio/conio++/vdc.h>

namespace vxio::conio
{





/*!
 * @brief widget ...
 */
class CONIO_DLL widget : public object
{
    vdc*        _dc = nullptr;
    vxio::size  _sz;        ///< This widget's dimensions.
    vxy         _position;  ///< widget's position relative to the parent widget
    
    friend class ts;
    friend class vdc;
protected:
    enum type: uint16_t
    {
        toplevel,
        child
        //..
        
    };
    
    widget::type _class = widget::type::toplevel; ///< = have owned vdc.
    
public:
    widget() = default;
    widget(widget* parent_);
    
    ~widget() override;
    
    virtual void set_geometry(vxy xy_, size sz_);
    
    
};

}

