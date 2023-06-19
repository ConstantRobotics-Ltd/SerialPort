#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <vector>
#include "SerialPort.h"

/// Link namespaces.
using namespace std;
using namespace cr::clib;
using namespace std::chrono;

// Entry point.
int main(void)
{
    cout<< "================================================" << endl;
    cout<< "Serial port string tester v" << SerialPort::getVersion() << endl;
    cout<< "================================================" << endl;
    cout<< endl;

    // Set serial port name.
    string serialPortName = "";
    cout << "Set serial port name: ";
    cin >> serialPortName;

    // Set baudrate.
    int baudrate = 0;
    cout << "Set baudrate: ";
    cin >> baudrate;

    // Open serial port.
    SerialPort serialPort;
    if (!serialPort.open(serialPortName.c_str(), baudrate, 0))
    {
        cout << "ERROR: Serial port not open. Exit" << endl;
        this_thread::sleep_for(seconds(1));
        return -1;
    }

    // Init output data buffer.
    const int bufferSize = 512;
    uint8_t buffer[bufferSize];

    // Main loop.
    while (true)
    {
        // Clear input buffer.
        while (serialPort.readData(buffer, bufferSize) > 0)
            serialPort.readData(buffer, bufferSize);

        // Enter string.
        string dataStr = "";
        cout << "Enter string: ";
        cin >> dataStr;
        dataStr += '\r';
        dataStr += '\n';

        // Copy to buffer.
        int bytes = (int)dataStr.size();
        memcpy(buffer, dataStr.data(), bytes);

        // Show output data.
        cout << "[TX]: ";
        for (int i = 0; i < bytes; ++i)
            cout << hex << (int)buffer[i] << " ";
        cout << dec << endl;

        // Send data.
        if (serialPort.sendData(buffer, bytes) != bytes)
        {
            cout << "ERROR: Can't send data" << endl;
            continue;
        }

        // Wait.
        this_thread::sleep_for(seconds(2));

        // Wait data from serial port. Wait 1 sec.
        bytes = serialPort.readData(buffer, bufferSize);
        if (bytes <= 0)
        {
            cout << "ERROR: No response from serial port" << endl;
            continue;
        }

        // Show input data.
        cout << "[RX]: ";
        for (int i = 0; i < bytes; ++i)
            cout << hex << (int)buffer[i] << " ";
        cout << "  :  " << string(buffer, buffer + bytes) << endl;
    }

    return 1;
}
