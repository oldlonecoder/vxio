//
// Created by oldlonecoder on 21-11-23.
//


#include <vxio/util/logger.h>
#include <vxio/conio++/conio++.h>



auto main() -> int {
    using vxio::conio::ts;
    using vxio::color;
    iostr str;
    
    ts::init();
    str << color::Aquamarine3 << " Terminal geometry:" << color::Yellow << vxio::rectangle({ 0,0 }, ts::sz).to_string() << color::Reset << "\n";
    ts::put(str());
    return 0;
}