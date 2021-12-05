//
// Created by oldlonecoder on 21-11-20.
//

//#ifndef VXIO_FRAMEWORK_OBJECT_H
//#define VXIO_FRAMEWORK_OBJECT_H
#pragma once
#include <vxio/dll_config.h>
#include <vector>


namespace vxio
{


class VXIO_API_DLL object
{
    object* _parent = nullptr;
public:
    using collection = std::vector<object*>;
    
    object();
    explicit object(object* parent_);
    
    virtual ~object();
    
    template<typename T=object> T* parent()
    {
        object* This = this;
        while(This->_parent)
        {
            This = This->_parent;
            if(T*o; o = dynamic_cast<T*>(This)) return o;
        }
        return nullptr;
    }

    int append_child(object* o_);
    int detach();
    collection::iterator  query_child(object*);
private:
    collection _children;
    
};

}


//#endif //VXIO_FRAMEWORK_OBJECT_H
