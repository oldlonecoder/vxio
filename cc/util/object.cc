//
// Created by oldlonecoder on 21-11-20.
//

#include <vxio/util/object.h>

namespace vxio
{



object::object()
= default;



object::object(object* parent_)
{
    if(parent_)
    {
        _parent = parent_;
        _parent->append_child(this);
    }

}

object::~object()
{

}

int object::append_child(object* o_)
{
    _children.push_back(o_);
    return (int)_children.size();
}

int object::detach()
{
    if(!_parent)
        return 0;
    
    if(collection::iterator i; (i = _parent->query_child(this)) != _parent->_children.end())
    {
        _parent->_children.erase(i);
        this->_parent = nullptr;
    }

    return 0;
}

object::collection::iterator object::query_child(object* o_)
{
    for( auto i = _children.begin(); i != _children.end(); i++)
        if(*i ==  o_) return i;
    return _children.end();
}


}