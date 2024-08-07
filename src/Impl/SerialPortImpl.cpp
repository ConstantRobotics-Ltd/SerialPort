﻿#define _CRT_SECURE_NO_WARNINGS
#include "SerialPortImpl.h"
#include <cstring>
#include <chrono>
#include <ctime>
#include <thread>
#include <iostream>



// Link namespaces.
using namespace cr::clib;




SerialPortImpl::SerialPortImpl()
{

}



SerialPortImpl::~SerialPortImpl()
{
    close();
}



void SerialPortImpl::close()
{
    if (m_initFlag)
#if defined(linux) || defined(__linux) || defined(__linux__)|| defined(__FreeBSD__)
    {
        ::close(m_port);
        flock(m_port, LOCK_UN);
    }
    m_initFlag = false;
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
        CloseHandle(m_port);
    m_initFlag = false;
#endif

}



int SerialPortImpl::read(uint8_t *buf, uint32_t size)
{
    if (!m_initFlag)
        return -1;

#if defined(linux) || defined(__linux) || defined(__linux__)|| defined(__FreeBSD__)

    if (m_timeoutMs > 0)
    {
        fd_set read_fds;
        struct timeval timeout;
        FD_ZERO(&read_fds);
        FD_SET(m_port, &read_fds);
        timeout.tv_sec = m_timeoutMs / 1000;
        timeout.tv_usec = (m_timeoutMs % 1000) * 1000;
        int ret = select(m_port + 1, &read_fds, NULL, NULL, &timeout);
        if (ret > 0) 
        {
            return ::read(m_port, buf, size);
        } 
        else if (ret == 0) 
        {
            // Timeout
            return 0;
        } 
        else 
        {
            // Error
            return -1;
        }
    }
    else
    {
        return ::read(m_port, buf, size);
    }
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

    int n = 0;
    ClearCommError(m_port, NULL, NULL);
    if (ReadFile(m_port, buf, size, (LPDWORD)((void*)&n), NULL))
        return n;
        
#endif

    return -1;
}



int SerialPortImpl::write(uint8_t *buf, uint32_t size)
{
    if (!m_initFlag)
        return -1;

#if defined(linux) || defined(__linux) || defined(__linux__)|| defined(__FreeBSD__)
    return ::write(m_port, buf, size);
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    int n;
    if (WriteFile(m_port, buf, size, (LPDWORD)((void*)&n), NULL))
        return n;
#endif

    return -1;
}



bool SerialPortImpl::open(
    std::string comport_file,
    unsigned int baudrate,
    unsigned int timeout,
    const char *mode) {

    if (m_initFlag)
        close();

    // Copy timeout.
    m_timeoutMs = timeout;

#if defined(linux) || defined(__linux) || defined(__linux__)|| defined(__FreeBSD__)

    bool isSBUS{false};
    int baudr = 0;
    switch (baudrate)
    {
    case 50: baudr = B50; break;
    case 75: baudr = B75; break;
    case 110: baudr = B110; break;
    case 134: baudr = B134; break;
    case 150: baudr = B150; break;
    case 200: baudr = B200; break;
    case 300: baudr = B300; break;
    case 600: baudr = B600; break;
    case 1200: baudr = B1200; break;
    case 1800: baudr = B1800; break;
    case 2400: baudr = B2400; break;
    case 4800: baudr = B4800; break;
    case 9600: baudr = B9600; break;
    case 19200: baudr = B19200; break;
    case 38400: baudr = B38400; break;
    case 57600: baudr = B57600; break;
    case 115200: baudr = B115200; break;
    case 230400: baudr = B230400; break;
    case 460800: baudr = B460800; break;
    case 500000: baudr = B500000; break;
    case 576000: baudr = B576000; break;
    case 921600: baudr = B921600; break;
    case 1000000: baudr = B1000000; break;
    case 1152000: baudr = B1152000; break;
    case 1500000: baudr = B1500000; break;
    case 2000000: baudr = B2000000; break;
    case 2500000: baudr = B2500000; break;
    case 3000000: baudr = B3000000; break;
    case 3500000: baudr = B3500000; break;
    case 4000000: baudr = B4000000; break;
    case 100000: isSBUS = true; break;
    default: return false;
    }

    if (strlen(mode) != 3)
        return false;

    int cbits = CS8;
    switch (mode[0]) {
    case '8':
        cbits = CS8;
        break;
    case '7':
        cbits = CS7;
        break;
    case '6':
        cbits = CS6;
        break;
    case '5':
        cbits = CS5;
        break;
    default:
        return false;
    }

    int	cpar = 0;
    int	ipar = IGNPAR;
    switch (mode[1]) {
    case 'N': case 'n':
        cpar = 0;
        ipar = IGNPAR;
        break;
    case 'E': case 'e':
        cpar = PARENB;
        ipar = INPCK;
        break;
    case 'O': case 'o':
        cpar = (PARENB | PARODD);
        ipar = INPCK;
        break;
    default:
        return false;
    }

    int	bstop = 0;
    switch (mode[2]) {
    case '1':
        bstop = 0;
        break;
    case '2':
        bstop = CSTOPB;
        break;
    default:
        return false;
    }

    m_port = ::open(comport_file.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (m_port == -1)
        return false;

    if (flock(m_port, LOCK_EX | LOCK_NB) != 0) {
        close();
        return false;
    }

    if(isSBUS)
    {
        struct termios port_settings;
        memset(&port_settings, 0, sizeof(port_settings));
        port_settings.c_cflag = CS8 | CLOCAL | CREAD; // control mode flags.
        port_settings.c_iflag = INPCK | PARENB | CSTOPB; // input mode flags
        port_settings.c_ispeed = 1000;
        port_settings.c_ospeed = 1000;
        if (tcsetattr(m_port, TCSANOW, &port_settings) == -1) 
        {
            return false;
        }

        m_initFlag = true;

        return true;
    }

    struct termios port_settings;
    memset(&port_settings, 0, sizeof(port_settings));
    port_settings.c_cflag = cbits | cpar | bstop | CLOCAL | CREAD; // control mode flags.
    port_settings.c_iflag = ipar; // input mode flags
    port_settings.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off software based flow control (XON / XOFF).
    port_settings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); // NON standard mode.
    port_settings.c_oflag = 0; // output mode flags  */
    port_settings.c_lflag = 0; // local mode flags   */
    port_settings.c_cc[VMIN] = 0;
    port_settings.c_cc[VTIME] = 0;

    cfsetispeed(&port_settings, baudr);
    cfsetospeed(&port_settings, baudr);

    if (tcsetattr(m_port, TCSANOW, &port_settings) == -1) {
        close();
        return false;
    }

    m_initFlag = true;

    return true;

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

    char mode_str[128];

    switch (baudrate) {
    case 110: strcpy(mode_str, "baud=110"); break;
    case 300: strcpy(mode_str, "baud=300"); break;
    case 600: strcpy(mode_str, "baud=600"); break;
    case 1200: strcpy(mode_str, "baud=1200"); break;
    case 2400: strcpy(mode_str, "baud=2400"); break;
    case 4800: strcpy(mode_str, "baud=4800"); break;
    case 9600: strcpy(mode_str, "baud=9600"); break;
    case 19200: strcpy(mode_str, "baud=19200"); break;
    case 38400: strcpy(mode_str, "baud=38400"); break;
    case 57600: strcpy(mode_str, "baud=57600"); break;
    case 115200: strcpy(mode_str, "baud=115200"); break;
    case 128000: strcpy(mode_str, "baud=128000"); break;
    case 256000: strcpy(mode_str, "baud=256000"); break;
    case 460800: strcpy(mode_str, "baud=460800"); break;
    case 500000: strcpy(mode_str, "baud=500000"); break;
    case 921600: strcpy(mode_str, "baud=921600"); break;
    case 1000000: strcpy(mode_str, "baud=1000000"); break;
    default:
        return false;
    }

    if (strlen(mode) != 3)
        return false;

    switch (mode[0]) {
    case '8':
        strcat(mode_str, " data=8");
        break;
    case '7':
        strcat(mode_str, " data=7");
        break;
    case '6':
        strcat(mode_str, " data=6");
        break;
    case '5':
        strcat(mode_str, " data=5");
        break;
    default: return false;
    }

    switch (mode[1]) {
    case 'N': case 'n':
        strcat(mode_str, " parity=n");
        break;
    case 'E': case 'e':
        strcat(mode_str, " parity=e");
        break;
    case 'O': case 'o':
        strcat(mode_str, " parity=o");
        break;
    default:
        return false;
    }

    switch (mode[2]) {
    case '1':
        strcat(mode_str, " stop=1");
        break;
    case '2':
        strcat(mode_str, " stop=2");
        break;
    default:
        return false;
    }

    strcat(mode_str, " dtr=on rts=on");

    m_port = CreateFileA(comport_file.c_str(),
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         NULL,
                         OPEN_EXISTING,
                         NULL,
                         NULL);
    if (m_port == INVALID_HANDLE_VALUE)
        return false;

    DCB port_settings = {0};
    port_settings.DCBlength = sizeof(port_settings);

    if (!BuildCommDCBA(mode_str, &port_settings))
    {
        close();
        return false;
    }

    if (!SetCommState(m_port, &port_settings))
    {
        close();
        return false;
    }

    COMMTIMEOUTS Cptimeouts = {0};
    Cptimeouts.ReadIntervalTimeout = MAXDWORD;
    Cptimeouts.ReadTotalTimeoutConstant = timeout;
    Cptimeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
    if (!SetCommTimeouts(m_port, &Cptimeouts))
    {
        close();
        return false;
    }

    m_initFlag = true;

    return true;

#endif

}



bool SerialPortImpl::isOpen()
{
    return m_initFlag;
}



bool SerialPortImpl::setFlowControl(bool enable)
{
    if (!m_initFlag)
        return false;

#if defined(linux) || defined(__linux) || defined(__linux__)|| defined(__FreeBSD__)
    struct termios tty;

    // Read in existing settings, and handle any error
    // NOTE: This is important! POSIX states that the struct passed to tcsetattr()
    // must have been initialized with a call to tcgetattr() overwise behaviour
    // is undefined
    if(tcgetattr(m_port, &tty) != 0)
        return false;

    if (enable)
        tty.c_cflag |= CRTSCTS;  // Enable RTS/CTS hardware flow control
    else
        tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)


    if (tcsetattr(m_port, TCSANOW, &tty) == -1) {
        return false;
    }

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    if (enable)
    {
        return EscapeCommFunction(m_port, SETRTS);
    }
    else
    {
        return EscapeCommFunction(m_port, CLRRTS);
    }
#endif
    return true;
}