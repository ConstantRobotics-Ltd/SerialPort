#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include "SerialPort.h"

using namespace std;
using namespace cr::clib;
using namespace std::chrono;

int main(void)
{
    cout << "Data sender example v" << SerialPort::getVersion() << endl;

    int portNum = 0;
    cout << "Enter serial port num: ";
    cin >> portNum;

    int portBaudrate = 0;
    cout << "Enter serial port baudrate: ";
    cin >> portBaudrate;

    int numBytesToSend = 0;
    cout << "Enter num bytes to send: ";
    cin >> numBytesToSend;

    int cyclePeriodMs = 0;
    cout << "Enter sending data period ms: ";
    cin >> cyclePeriodMs;

    // Define serial port name.
#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__FreeBSD__)
    std::string portName = "/dev/ttyS" + std::to_string(portNum);
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    string portName = "\\\\.\\COM" + to_string(portNum);
#endif

    // Open serial port.
    SerialPort serialPort;
    if (!serialPort.open(portName, portBaudrate))
        return -1;

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
        std::cout << serialPort.write(outputData, numBytesToSend) <<
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