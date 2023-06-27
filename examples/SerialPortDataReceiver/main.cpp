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
        int bytes = serialPort.read(inputData, inputDataSize);

        // Check input data size.
        if (bytes <= 0)
            cout << "No input data" << endl;
        else
            cout << bytes << " bytes read." << endl;
    }
}


