//
// Created by oldlonecoder on 21-12-13.
//

//#ifndef VXIO_EXPECT_H
//#define VXIO_EXPECT_H

/**********************************************************************************************************************************
    Copyright 2021 Serge Lussier (oldlonecoder), lussier.serge@gmail.com
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
    THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*************************************************************************************************************************************/


#pragma once
#include <vxio/util/rem.h>
#include <any>

template<class T=rem::code> class expect
{
    std::any value;
    bool state = false;

public:
    expect() = default;
    
    expect(rem &m)
    {
        //rem msg = m;
        value = m;
        state   = false;
    }
    
    expect(const T &v)
    {
        value = v; // Must remove const ?
        state   = true;
    }
    
    expect(expect &&e) noexcept
    {
        value = std::move(e.value);
        state   = e.state;
    }
    
    expect(const expect &e)
    {
        state = e.state;
        value = e.value;
    }
    
    expect &operator=(rem &m)
    {
        value.reset();
        state  = false;
        rem msg = m;
        value = std::move(msg);
        return *this;
    }
    
    expect &operator=(expect &&e) noexcept
    {
        value.reset();
        value = std::move(e.value);
        state = std::move(e.state);
        return *this;
    }
    
    expect &operator=(const expect &e)
    {
        if(&e == this)
            return *this;
        value.reset();
        value = e.value;
        state = e.state;
        return *this;
    }
    
    expect &operator=(const T &v)
    {
        //        /if(mF)
        value.reset();
        value = v;
        state = true;
        return *this;
    }
    
    explicit operator bool() const
    { return state; }
    
    rem operator()()
    {
        if(!state)
            return std::any_cast<rem&>(value);
        return std::any_cast<rem>(value);
    }
    
    auto &operator*()
    {
        if(!state)
        {
            value.reset();
            throw rem( rem::type::err ) << ": " << " - Expected value was not returned. >>\n >> ";
        }

        return std::any_cast<T&>(value);
    }
    
    void reset()
    {
        value.reset();
        state = false;
    }
    
    ~expect()
    {
        //Reset();
    }
    
};



//#endif //VXIO_EXPECT_H
