## CONTENTS
- [OVERVIEW](#overview)
- [API DESCRIPTION](#api-description)
  - [getVersion(..)](#getversion)
  - [SerialPort(..)](#serialport)
  - [open(..)](#open)
  - [readData(..)](#readdata)
  - [sendData(..)](#senddata)
  - [isOpen(..)](#isopen)
  - [close(..)](#close)
  - [setFlowControl(..)](#setflowcontrol)
- [USAGE EXAMPLE](#usage-example)
  - [Receiver Example](#receiver-example)
  - [Sender Example](#sender-example)
  

## OVERVIEW

This project makes it easy to work with a Serial Port to create communication applications.

## API DESCRIPTION
The **SerialPort** contains one class `SerialPort` which can be used 
to simplify sending and receiving data over the serial transport layer. 

A SerialPort can be configured as a receiver port, which will listen 
for data on a specific port, or a sender, which will send data to a 
specific port. 

### getVersion(..)

```cpp
/**
 * @brief Method to get string of current version of library.
 *
 * @return String of current library version.
 */
static std::string getVersion();
```

##### Description

*Static method to get string of current version of library.*


### SerialPort(..)

```cpp
/**
 * @brief Class constructor.
 */
SerialPort();

/**
 * @brief Class destructor.
 */
~SerialPort();
```

##### Description

*Class constructor and destructor*

### open(..)

```cpp
/**
 * @brief Method to open serial port.
 *
 * @param comport_file Serial port name string. Format depends from OS.
 * @param baudrate Boudrate.
 * @param timeout Wait data timeout.
 * @param mode Mode. Always 3 simbols:
 * 1 - Number of bits (8, 7, 6, 5),
 * 2 - parity (N, E, O),
 * 3 - number of stop bits (1 or 2).
 * Example: "8N1".
 *
 * @return TRUE in case success or FALSE in case any errors.
 */
 bool open(const char *comport_file, unsigned int baudrate,
           unsigned int timeout = 100, const char *mode = "8N1");
```

##### Description

* Method to open serial port*

> In **Windows** serial ports are named `\\\\.\\COM*`.

> In typical **UNIX** style, serial ports are represented by files within the operating system. These files usually pop-up in `/dev/`, and begin with the name `tty*`.
> Common names are:
> **/dev/ttyACM0** - ACM stands for the ACM modem on the USB bus. Arduino UNOs (and similar) will appear using this name.
> **/dev/ttyPS0** - Xilinx Zynq FPGAs running a Yocto-based Linux build will use this name for the default serial port that Getty connects to.
> **/dev/ttyS0** - Standard COM ports will have this name. These are less common these days with newer desktops and laptops not having actual COM ports.
> **/dev/ttyUSB0** - Most USB-to-serial cables will show up using a file named like this.
> **/dev/pts/0** - A pseudo terminal. These can be generated with socat.

### readData(..)

```cpp
/**
 * @brief Read data from serial port.
 *
 * @param buf pointer to data buffer to copy.
 * @param size size of data buffer.
 *
 * @return Number of read  bytes or returns -1.
 */
int readData(uint8_t *buf, uint32_t size);
```

##### Description

*Method to read data.*

> This method will return the received data in two cases: when the buffer is full, or when the timeout ends for receiving data.

### sendData(..)

```cpp
/**
 * @brief Send data to serial port.
 *
 * @param buf pointer to data to send.
 * @param size size of data to send.
 *
 * @return Number of bytes sended or return -1 in case any errors.
 */
int sendData(uint8_t *buf, uint32_t size);
```

##### Description

*Method to send data.*

### isOpen(..)

```cpp
/**
 * @brief Method to check if serial port open.
 * @return TRUE if port open or FALSE.
 */
bool isOpen();
```

##### Description

*Method to check if serial port open.*

### close(..)

```cpp
/**
 * @brief Method to close serial port.
 */
void close();
```

##### Description

*Method to close serial port.*

### setFlowControl(..)

```cpp
/**
 * @brief Method to set RTS/CTS hardware flow control
 *
 * @param enable Enable RTS/CTS hardware flow control
 *
 * @return TRUE in case success or FALSE in case any errors.
 */
bool setFlowControl(bool enable);
```

##### Description

*Method to set RTS/CTS hardware flow control.*

## USAGE EXAMPLE

### Receiver Example

```cpp
#include "SerialPort.h"
using namespace cr::clib;

int main(void)
{
    SerialPort receiver;

    // serial port num.
    int portNum = 10;
    // serial port baudrate.
    int portBaudrate = 115200;
    // wait data timeout.
    int waitDataTimeoutMs = 300;

    // Open serial port.
#if defined(linux) || defined(__linux) || defined(__linux__)|| defined(__FreeBSD__)
    std::string portName = "/dev/ttyS" + std::to_string(portNum);
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    std::string portName = "\\\\.\\COM" + std::to_string(portNum);
#endif

    if (!receiver.open(portName.c_str(), portBaudrate, waitDataTimeoutMs))
    {
        std::cerr << "SerialPort can not open!" << std::endl;
        return -1;
    }

    // Read data from sender.
    const uint16_t inputDataSize = 512; //< Any value.
    uint8_t* inputData = new uint8_t[inputDataSize];
    while (true)
    {
        int bytes = receiver.readData(inputData, inputDataSize);
        if (bytes <= 0) {
            std::cout << "No data received..." << std::endl;
            continue;
        }

        std::cout << bytes << " bytes were received" << std::endl;

        // Send received data to the sender.
        bytes = receiver.sendData(inputData, bytes);
        if (bytes > 0) 
            std::cout << bytes << " bytes were sent" << std::endl;
        else 
            std::cerr << "Data sending error!" << std::endl;
    }
    delete[] inputData;
}
```

### Sender Example

```cpp
#include "SerialPort.h"
using namespace cr::clib;

int main(void)
{
    SerialPort sender;

    // serial port num.
    int portNum = 11;
    // serial port baudrate.
    int portBaudrate = 115200;
    // wait data timeout.
    int waitDataTimeoutMs = 300;

    // Open serial port.
#if defined(linux) || defined(__linux) || defined(__linux__)|| defined(__FreeBSD__)
    std::string portName = "/dev/ttyS" + std::to_string(portNum);
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    std::string portName = "\\\\.\\COM" + std::to_string(portNum);
#endif

    if (!sender.open(portName.c_str(), portBaudrate, waitDataTimeoutMs))
    {
        std::cerr << "SerialPort can not open!" << std::endl;
        return -1;
    }

    // Send data to receiver.
    int testValue = 0;
    uint32_t dataSize = 512; //< Any value.
    uint8_t* data = new uint8_t[dataSize];
    while (true)
    {
        // Fill the data with an increasing value.
        memset(data, testValue++, dataSize);
        int bytes = sender.sendData(data, dataSize);
        if (bytes <= 0) {
            std::cerr << "Data sending error!" << std::endl;
            return -1;
        }

        std::cout << bytes << " bytes were sent" << std::endl;

        // Try to receive answer.
        bytes = sender.readData(data, dataSize);
        if (bytes > 0)
            std::cout << bytes << " bytes were received" << std::endl;
        else
            std::cerr << "No data received..." << std::endl;
    }
    delete[] data;
}
```
