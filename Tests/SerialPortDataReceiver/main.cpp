#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <SerialPort.h>

// Global constants.
constexpr int g_maxNumBytesToRead = 2048;

// Entry point.
int main(void)
{
    std::cout << "###  Serial port data receiver  ###" << std::endl << std::endl;

    // Enter serial port num.
    int serialPortNum = 0;
    std::cout << "Enter serial port num: ";
    std::cin >> serialPortNum;

    // Enter serial port baudrate.
    int serialPortBaudrate = 0;
    std::cout << "Enter serial port baudrate: ";
    std::cin >> serialPortBaudrate;

    // Enter sending data period ms.
    int waitDataTimeoutMs = 0;
    std::cout << "Enter wait data timeout ms: ";
    std::cin >> waitDataTimeoutMs;

    // Open serial port.
#if defined(linux) || defined(__linux) || defined(__linux__)|| defined(__FreeBSD__)
    std::string serialPortName = "/dev/ttyS" + std::to_string(serialPortNum);
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    std::string serialPortName = "\\\\.\\COM" + std::to_string(serialPortNum);
#endif
    cr::clib::SerialPort serialPort;
    if (!serialPort.open(serialPortName.c_str(), serialPortBaudrate, waitDataTimeoutMs))
    {
        std::cout << "ERROR: Serial port not open. Exit" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return -1;
    }

    // Init variables.
    uint8_t inputData[g_maxNumBytesToRead];

    // Main loop.
    while (true)
    {
        // Read data.
        int bytes = serialPort.readData(inputData, g_maxNumBytesToRead);

        // Check input data size.
        if (bytes <= 0)
        {
            std::cout << "No input data" << std::endl;
            continue;
        }

        // Display input data.
        std::cout << bytes << " bytes read: " ;
        for (int i = 0; i < bytes; ++i)
        {
            std::cout << std::hex << (int)inputData[i] << " ";
        }
        std::cout << std::endl << std::dec << std::endl;
    }
}


