#include <vxio/conio++/conio++.h>
#ifdef _WIN32
#   include <windows.h>
#else 
#   include <sys/ioctl.h>
#   include <termios.h>
#   include <unistd.h>
#endif

#include <vxio/util/logger.h>

namespace vxio::conio
{



vxio::size ts::sz;

rem::code ts::init()
{
    get_wh();
    //...
    return rem::code::ok;
}


rem::code ts::get_wh()
{
    int columns, rows;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    winsize tty;
    ioctl(0, TIOCGWINSZ, (uint8_t*)&tty);

    rows = tty.ws_row + 1;
    columns = tty.ws_col;
#endif
    ts::sz = { columns, rows };
    //...

    return rem::code::ok;
}


rem::code ts::tput(const std::string& line)
{
    auto r = ::write(1, line.c_str(), line.length());
    if (r != line.length())
    {
        logger::error() << " system stdout write: '" << strerror(errno) << "'\n";
        return rem::code::unexpected;
    }

    (void)fflush(stdout); // Probablement trop syst�matique.
    return rem::code::ok;
}


}
