#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>

#include "Tracer.h"
#include "SerialPort.h"

using namespace cr::clib;
using namespace cr::utils;

// Entry point.
int main(void)
{
    std::cout<< "=================================================" << std::endl;
    std::cout<< "SerialPortDataReceiver " <<SerialPort::getVersion()<< std::endl;
    std::cout<< "=================================================" << std::endl;
    std::cout<< "Library versions: "                                << std::endl;
    std::cout<< "Tracer:............"<< Tracer::getVersion()        << std::endl;
    std::cout<< "SerialPort:........."<< SerialPort::getVersion()   << std::endl;
    std::cout<< "-------------------------------------------------" << std::endl;
    std::cout<< std::endl;

    // Enter serial port num.
    int portNum = 0;
    std::cout << "Enter serial port num: ";
    std::cin >> portNum;

    // Enter serial port baudrate.
    int portBaudrate = 0;
    std::cout << "Enter serial port baudrate: ";
    std::cin >> portBaudrate;

    // Enter wait data timeout.
    int waitDataTimeoutMs = 0;
    std::cout << "Enter wait data timeout ms: ";
    std::cin >> waitDataTimeoutMs;

    // Open serial port.
#if defined(linux) || defined(__linux) || defined(__linux__)|| defined(__FreeBSD__)
    std::string portName = "/dev/ttyS" + std::to_string(portNum);
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    std::string portName = "\\\\.\\COM" + std::to_string(portNum);
#endif

    // Init serial port.
    cr::clib::SerialPort serialPort;
    if (!serialPort.open(portName.c_str(), portBaudrate, waitDataTimeoutMs))
    {
        std::cout << "ERROR: Serial port not open. Exit" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
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
            std::cout << "No input data" << std::endl;
        else
            std::cout << bytes << " bytes read." << std::endl;
    }
}


