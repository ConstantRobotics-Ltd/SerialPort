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
    cout << "Data receiver example v" << SerialPort::getVersion() << endl;

    string portName = "";
#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__FreeBSD__)
    cout << "Enter serial port name (e.g. /dev/ttyS0): ";
    cin >> portName;
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    //cout << "Enter serial port num (1, 2 ..): ";
    //cin >> portName;
    //portName = "\\\\.\\COM" + portName;
    portName = "\\\\.\\COM5";
#endif

    int portBaudrate = 115200;
    //cout << "Enter serial port baudrate: ";
    //cin >> portBaudrate;

    int waitDataTimeoutMs = 0;
    cout << "Enter wait data timeout, msec: ";
    cin >> waitDataTimeoutMs;

    // Open serial port.
    SerialPort serialPort;
    if (!serialPort.open(portName, portBaudrate, waitDataTimeoutMs))
        return -1;

    // Main loop.
    uint8_t inputData[1024];
    while (true)
    {
        // Read data.
        int bytes = serialPort.read(inputData, 1024);

        if (bytes <= 0)
            cout << "No input data" << endl;
        else
            cout << bytes << " bytes read." << endl;
    }
    return 1;
}