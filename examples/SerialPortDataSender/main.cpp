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

    string portName = "";
#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__FreeBSD__)
    cout << "Enter serial port name (e.g. /dev/ttyS0): ";
    cin >> portName;
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    cout << "Enter serial port num (1, 2 ..): ";
    cin >> portName;
    portName = "\\\\.\\COM" + portName;
#endif
    
    int portBaudrate = 115200;
    cout << "Enter serial port baudrate: ";
    cin >> portBaudrate;

    int numBytesToSend = 10;
    cout << "Enter num bytes to send: ";
    cin >> numBytesToSend;
    if (numBytesToSend > 1024)
        return -1;

    int cyclePeriodMs = 0;
    cout << "Enter sending data period ms: ";
    cin >> cyclePeriodMs;

    // Open serial port.
    SerialPort serialPort;
    if (!serialPort.open(portName, portBaudrate))
        return -1;
    
    // Main loop.
    uint8_t outputData[1024];
    time_point<system_clock> start = system_clock::now();
    while (true)
    {
        // Wait according to parameters.
        int waitTime = cyclePeriodMs -
        (int)duration_cast<milliseconds>(system_clock::now() - start).count();
        while (waitTime > 0)
            waitTime = cyclePeriodMs -
            (int)duration_cast<milliseconds>(system_clock::now() - start).count();
        start = system_clock::now();

        // Prepare random data.
        for (int i = 0; i < numBytesToSend; ++i)
            outputData[i] = (uint8_t)(rand() % 255);

        // Send data.
        cout << serialPort.write(outputData, numBytesToSend) <<
        " bytes sent" << endl;
    }

    return 1;
}