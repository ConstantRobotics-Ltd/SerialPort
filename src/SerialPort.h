#pragma once
#include <string>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <atomic>
#if defined(linux) || defined(__linux) || defined(__linux__)
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/file.h>
#include <stdint.h>
#include <mutex>
#else
#include <windows.h>
#endif



namespace cr
{
namespace clib
{

/**
 * @brief Serial port class.
 */
class SerialPort
{
public:

    /**
     * @brief Get string of current library version.
     * @return String of current library version "Major.Minor.Patch"
     */
    static std::string getVersion();

    /**
     * @brief Class constructor.
     */
    SerialPort();

    /**
     * @brief Class destructor.
     */
    ~SerialPort();

    /**
     * @brief Open serial port.
     * @param file Serial port device name. Format depends from OS.
     *             For Linux OS format: "/dev/serial/by-id/...", "/dev/ttyS0",
     *             "/dev/ttyUSB0" etc. For Windows OS format "\\\\.\\COM0" etc.
     * @param baudrate Boudrate. For example 115200.
     * @param timeoutMsec Wait data timeout, milisseconds.
     * @param mode Mode. Always 3 simbols:
     *             1 - Number of bits (8, 7, 6, 5),
     *             2 - parity (N, E, O),
     *             3 - number of stop bits (1 or 2).
     *             Example: "8N1".
     * @return TRUE in case success or FALSE in not.
     */
    bool open(std::string file, unsigned int baudrate,
              unsigned int timeoutMsec = 100, const char *mode = "8N1");

    /**
     * @brief Read data from serial port. The method will wait timeout specified
     *        in open(...) method.
     * @param buf pointer to data buffer to copy.
     * @param size size of data buffer.
     * @return Number of readed bytes or returns -1 if no input data.
     */
    int read(uint8_t *buf, uint32_t size);

    /**
     * @brief Write data to serial port.
     * @param buf pointer to data to send.
     * @param size size of data to send.
     * @return Number of bytes sended or return -1 in case any errors.
     */
    int write(uint8_t *buf, uint32_t size);

    /**
     * @brief Get open status.
     * @return TRUE if port open or FALSE if not.
     */
    bool isOpen();

    /**
     * @brief Close serial port.
     */
    void close();

    /**
     * @brief Set RTS/CTS hardware flow control.
     * @param enable Enable RTS/CTS hardware flow control.
     * @return TRUE in case success or FALSE if not.
     */
    bool setFlowControl(bool enable);

private:

    /// Port initialization flag.
    std::atomic<bool> m_initFlag{false};
    /// Read data timeout.
    int m_timeoutMs{0};
    /// Port handle.
#if defined(linux) || defined(__linux) || defined(__linux__)|| defined(__FreeBSD__)
    int m_port{0};
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    HANDLE m_port{0};
#endif

};
}
}
