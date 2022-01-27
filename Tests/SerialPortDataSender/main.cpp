#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include "SerialPort.h"

// Entry point.
int main(void)
{
    std::cout << "###  Serial port data sender  ###" << std::endl << std::endl;

    // Enter serial port num.
    int serialPortNum = 0;
    std::cout << "Enter serial port num: ";
    std::cin >> serialPortNum;

    // Enter serial port baudrate.
    int serialPortBaudrate = 0;
    std::cout << "Enter serial port baudrate: ";
    std::cin >> serialPortBaudrate;

    // Enter numer of bytes.
    int numBytesToSend = 0;
    std::cout << "Enter num bytes to send: ";
    std::cin >> numBytesToSend;

    // Enter sending data period ms.
    int cyclePeriodMs = 0;
    std::cout << "Enter sending data period ms: ";
    std::cin >> cyclePeriodMs;

    // Open serial port.
#if defined(linux) || defined(__linux) || defined(__linux__)|| defined(__FreeBSD__)
    std::string serialPortName = "/dev/ttyS" + std::to_string(serialPortNum);
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    std::string serialPortName = "\\\\.\\COM" + std::to_string(serialPortNum);
#endif
    cr::clib::SerialPort serialPort;
    if (!serialPort.open(serialPortName.c_str(), serialPortBaudrate))
    {
        std::cout << "ERROR: Serial port not open. Exit" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return -1;
    }

    // Init variables.
    uint8_t* outputData = new uint8_t[numBytesToSend];

    // Main loop.
    std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();
    while (true)
    {
        // Prepare random data.
        for (int i = 0; i < numBytesToSend; ++i)
            outputData[i] = (uint8_t)(rand() % 255);

        // Send data.
        std::cout << serialPort.sendData(outputData, numBytesToSend) << "bytes sent" << std::endl;

        // Wait according to parameters.
        int waitTime = (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count();
        waitTime = cyclePeriodMs - waitTime;
        if (waitTime > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
        startTime = std::chrono::system_clock::now();
    }

	return 1;
}
