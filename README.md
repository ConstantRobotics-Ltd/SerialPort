



![logo](_static/serial_port_logo.png)

# **SerialPort C++ library**

**v2.3.0**

------

# Table of contents

- [Overview](#Overview)
- [Versions](#Versions)
- [SerialPort class description](#SerialPort-class-description)
  - [Class declaration](#Class-declaration)
  - [getVersion method](#getVersion-method)
  - [open method](#open-method)
  - [readData method](#readData-method)
  - [sendData method](#sendData-method)
  - [isOpen method](#isOpen-method)
  - [close method](#close-method)
  - [setFlowControl method](#setFlowControl-method)
- [Examples](#Examples)
  - [Data sender example](#Data-sender-example)
  - [Data receiver example](#Data-receiver-example)

# Overview

**SerialPort** C++ library provides simple interface to work with serial port. **SerialPort.h** file contains declaration of **SerialPort** C++ class. **SerialPort** has only to functions: open, send data and read data from serial port.

# Versions

**Table 1** - Library versions.

| Version | Release date | What's new                                                   |
| ------- | ------------ | ------------------------------------------------------------ |
| 1.0.0   | 14.10.2021   | First version                                                |
| 2.0.0   | 20.05.2022   | - Interface changed.<br />- Added method to RTS/CTS hardware flow control. |
| 2.1.0   | 10.07.2022   | - Code optimized.                                            |
| 2.2.0   | 22.03.2023   | - Code optimized.<br />- Documentation updated.              |
| 2.3.0   | 24.04.2023   | - Added new test application.                                |

# SerialPort class description

## Class declaration

**SerialPort** class declared in **SerialPort.h** file. Class declaration:

```cpp
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
     * @brief Method to get string of current library version.
     * @return String of current library version.
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
     * @brief Method to open serial port.
     * @param comport_file Serial port name string. Format depends from OS.
     * @param baudrate Boudrate.
     * @param timeout Wait data timeout.
     * @param mode Mode. Always 3 simbols:
     * 1 - Number of bits (8, 7, 6, 5),
     * 2 - parity (N, E, O),
     * 3 - number of stop bits (1 or 2).
     * Example: "8N1".
     * @return TRUE in case success or FALSE in case any errors.
     */
    bool open(const char *file, unsigned int baudrate,
              unsigned int timeout = 100, const char *mode = "8N1");

    /**
     * @brief Read data from serial port.
     * @param buf pointer to data buffer to copy.
     * @param size size of data buffer.
     * @return Number of readed bytes or returns -1.
     */
    int readData(uint8_t *buf, uint32_t size);

    /**
     * @brief Send data to serial port.
     * @param buf pointer to data to send.
     * @param size size of data to send.
     * @return Number of bytes sended or return -1 in case any errors.
     */
    int sendData(uint8_t *buf, uint32_t size);

    /**
     * @brief Method to check if serial port open.
     * @return TRUE if port open or FALSE.
     */
    bool isOpen();

    /**
     * @brief Method to close serial port.
     */
    void close();

    /**
     * @brief Method to set RTS/CTS hardware flow control.
     * @param enable Enable RTS/CTS hardware flow control.
     * @return TRUE in case success or FALSE in case any errors.
     */
    bool setFlowControl(bool enable);
};
}
}
```

## getVersion method

**getVersion()** method return string of current version **SerialPort** class. Particular lens controller can have it's own **getVersion()** method. Method declaration:

```cpp
static std::string getVersion();
```

Method can be used without **Lens** class instance:

```cpp
std::cout << "Serial port version: " << cr::clib::SerialPort::getVersion() << std::endl;
```

## open method

**open(...)** method intended to open serial port. If serial port already open the method firstly will close serial port and will try open again according to method's parameters. Method declaration:

```cpp
bool open(const char *file, unsigned int baudrate, unsigned int timeout = 100, const char *mode = "8N1");
```

| Parameter | Value                                                        |
| --------- | ------------------------------------------------------------ |
| file      | Full serial port file name. In **Windows** serial ports are named **\\\\.\\COM**. In typical **UNIX** style, serial ports are represented by files within the operating system. These files usually pop-up in `/dev/`, and begin with the name `tty*`. Common names are:  <br />**/dev/ttyACM0**- ACM stands for the ACM modem on the USB bus. Arduino UNOs (and similar) will appear using this name.<br />**/dev/ttyPS0** - Xilinx Zynq FPGAs running a Yocto-based Linux build will use this name for the default serial port that Getty connects to.<br /> **/dev/ttyS0** - Standard COM ports will have this name. These are less common these days with newer desktops and laptops not having actual COM ports.<br />**/dev/ttyUSB0** *- Most USB-to-serial cables will show up using a file named like this.<br />**/dev/pts/0** - A pseudo terminal. These can be generated with socat. |
| baudrate  | Baudrate e.g. 2400, 4800, 9600, 19200, 38400, 57600, 115200 etc. |
| timeout   | Timeout in milliseconds for reading data from serial port. When user call **readData(...)** method it will wait **timeout** msec and will return all data from input serial port buffer. |
| mode      | Mode. Default "8N1" most common. Always 3 symbols:<br/>1 - Number of bits (8, 7, 6, 5),<br/>2 - parity (N, E, O),<br/>3 - number of stop bits (1 or 2). |

**Returns:** TRUE if the serial port open or FALSE if not.

## readData method

**readData(...)** method intended to read data from serial port. Method will wait **timeout** (set by user in **open(...)** method) and will return all data (<= requested amount of data) from input serial port buffer. If you don't want to wait and just check data in serial port or if you want to use different timeouts to wait data set **timeout = 0** in open(...) method. Method declaration:

```cpp
int readData(uint8_t *buf, uint32_t size);
```

| Parameter | Value                                                        |
| --------- | ------------------------------------------------------------ |
| buf       | Pointer to buffer.                                           |
| size      | Size of buffer. Size of data which you want to read from serial port. Method will return **<= size** bytes from input serial port buffer. |

**Returns:** number of received bytes **<= size** or **0** if no data in input serial port buffer or **-1** if serial port not open.

## sendData method

**sendData(...)** method intended to send data to serial port. Method declaration:

```cpp
int sendData(uint8_t *buf, uint32_t size);
```

| Parameter | Value                                |
| --------- | ------------------------------------ |
| buf       | Pointer to buffer with data to send. |
| size      | Number of bytes to send.             |

**Returns:** number of bytes sent **<= size** or **-1** if serial port not open.

## isOpen method

**isOpen()** method intended to obtain serial port connection status. Method declaration:

```cpp
bool isOpen();
```

**Returns:** TRUE is the serial port open or FALSE if not.

## close method

**close()** method intended to close serial port. Method declaration:

```cpp
void close();
```

## setFlowControl method

setFlowControl(...) method intended to set RTS/CTS hardware flow control. RTS / CTS Flow Control is **another flow control mechanism that is part of the RS232 standard**. It makes use of two further pins on the RS232 connector, RTS (Request to Send) and CTS (Clear to Send). These two lines allow the receiver and the transmitter to alert each other to their state. Method declaration:

```cpp
bool setFlowControl(bool enable);
```

| Parameter | Value                                                        |
| --------- | ------------------------------------------------------------ |
| enable    | Enable RTS/CTS hardware flow control flag: **true** - enable, **false** - disable. |

**Returns:** TRUE if mode was changed or FALSE.

# Examples

## Data sender example

```cpp
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include "SerialPort.h"

/// Link namespaces.
using namespace std;
using namespace cr::clib;
using namespace std::chrono;

// Entry point.
int main(void)
{
    cout<< "=================================================" << endl;
    cout<< "SerialPortDataSender " << SerialPort::getVersion() << endl;
    cout<< "=================================================" << endl;
    cout<< endl;

    // Enter serial port num.
    int portNum = 0;
    cout << "Enter serial port num: ";
    cin >> portNum;

    // Enter serial port baudrate.
    int portBaudrate = 0;
    cout << "Enter serial port baudrate: ";
    cin >> portBaudrate;

    // Enter numer of bytes.
    int numBytesToSend = 0;
    cout << "Enter num bytes to send: ";
    cin >> numBytesToSend;

    // Enter sending data period ms.
    int cyclePeriodMs = 0;
    cout << "Enter sending data period ms: ";
    cin >> cyclePeriodMs;

    // Open serial port.
#if defined(linux) || defined(__linux) || defined(__linux__)|| defined(__FreeBSD__)
    std::string portName = "/dev/ttyS" + std::to_string(portNum);
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    string portName = "\\\\.\\COM" + to_string(portNum);
#endif

    // Init serial port.
    SerialPort serialPort;
    if (!serialPort.open(portName.c_str(), portBaudrate))
    {
        cout << "ERROR: Serial port not open. Exit." << endl;
        this_thread::sleep_for(seconds(1));
        return -1;
    }

    // Init variables.
    uint8_t* outputData = new uint8_t[numBytesToSend];

    // Main loop.
    chrono::time_point<system_clock> startTime = system_clock::now();
    while (true)
    {
        // Prepare random data.
        for (int i = 0; i < numBytesToSend; ++i)
            outputData[i] = (uint8_t)(rand() % 255);

        // Send data.
        std::cout << serialPort.sendData(outputData, numBytesToSend) <<
                     " bytes sent" << std::endl;

        // Wait according to parameters.
        int waitTime = (int)duration_cast<std::chrono::milliseconds>(
                    system_clock::now() - startTime).count();
        waitTime = cyclePeriodMs - waitTime;
        if (waitTime > 0)
            this_thread::sleep_for(milliseconds(waitTime));
        startTime = system_clock::now();
    }

    return 1;
}
```

## Data receiver example

```cpp
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include "SerialPort.h"

/// Link namespaces.
using namespace std;
using namespace cr::clib;
using namespace std::chrono;

// Entry point.
int main(void)
{
    cout<< "==================================================" << endl;
    cout<< "SerialPortDataReceiver " << SerialPort::getVersion()<< endl;
    cout<< "==================================================" << endl;
    cout<< endl;

    // Enter serial port num.
    int portNum = 0;
    cout << "Enter serial port num: ";
    cin >> portNum;

    // Enter serial port baudrate.
    int portBaudrate = 0;
    cout << "Enter serial port baudrate: ";
    cin >> portBaudrate;

    // Enter wait data timeout.
    int waitDataTimeoutMs = 0;
    cout << "Enter wait data timeout ms: ";
    cin >> waitDataTimeoutMs;

    // Open serial port.
#if defined(linux) || defined(__linux) || defined(__linux__)|| defined(__FreeBSD__)
    std::string portName = "/dev/ttyS" + std::to_string(portNum);
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    string portName = "\\\\.\\COM" + to_string(portNum);
#endif

    // Init serial port.
    SerialPort serialPort;
    if (!serialPort.open(portName.c_str(), portBaudrate, waitDataTimeoutMs))
    {
        cout << "ERROR: Serial port not open. Exit" << endl;
        this_thread::sleep_for(seconds(1));
        return -1;
    }

    // Init variables.
    const uint16_t inputDataSize = 1024;
    uint8_t inputData[inputDataSize];

    // Main loop.
    while (true)
    {
        // Read data.
        int bytes = serialPort.readData(inputData, inputDataSize);

        // Check input data size.
        if (bytes <= 0)
            cout << "No input data" << endl;
        else
            cout << bytes << " bytes read." << endl;
    }
}
```











