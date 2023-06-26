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
    cout<< "Serial port tester v" << SerialPort::getVersion() << endl;
    cout<< "================================================" << endl;
    cout<< endl;

    // Set serial port name.
    string serialPortName = "";
#if defined(linux) || defined(__linux) || defined(__linux__)|| defined(__FreeBSD__)
    cout << "Set serial port name: ";
    cin >> serialPortName;
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    int portNum = 0;
    cout << "Set COM port num (1,2,3,...): ";
    cin >> portNum;
    serialPortName = "\\\\.\\COM" + to_string(portNum);
#endif

    // Set baudrate.
    int baudrate = 0;
    cout << "Set baudrate: ";
    cin >> baudrate;

    // Chose mode.
    int mode = 0;
    cout << "Chose mode (1 - string, 0 - HEX): ";
    cin >> mode;

    // Open serial port.
    SerialPort serialPort;
    if (!serialPort.open(serialPortName.c_str(), baudrate, 0))
    {
        cout << "ERROR: Serial port not open. Exit" << endl;
        this_thread::sleep_for(seconds(1));
        return -1;
    }

    // Init output data buffer.
    const int bufferSize = 128;
    uint8_t buffer[bufferSize];
    int bytes = 0;

    // Main loop.
    while (true)
    {
        // Clear input buffer.
        while (serialPort.readData(buffer, bufferSize) > 0)
            serialPort.readData(buffer, bufferSize);

        // Enter message.
        if (mode == 1)
        {
            // Enter string.
            string dataStr = "";
            cout << "Enter string: ";
            cin >> dataStr;
            dataStr += '\r';
            dataStr += '\n';

            // Copy to buffer.
            bytes = (int)dataStr.size();
            memcpy(buffer, dataStr.data(), bytes);
        }
        else
        {
            // Enter HEX string.
            string dataStr = "";
            cout << "Enter HEX string (e.g. AAF00A): ";
            cin >> dataStr;

            // Convert string to byte array.
            if (dataStr.length() % 2 != 0)
            {
                cout << "ERROR: Not valid number of symbols" << endl;
                continue;
            }
            bytes = (int)dataStr.length() / 2;
            for (int i = 0, j = 0; j < bytes; i += 2, j++)
                buffer[j] = (dataStr.c_str()[i] % 32 + 9) % 25 * 16 +
                            (dataStr.c_str()[i+1] % 32 + 9) % 25;
        }

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
        cout << dec;
        if (mode == 1)
            cout << "  :  " << string(buffer, buffer + bytes);
        cout << endl;
    }

    return 1;
}
