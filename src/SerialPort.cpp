#define _CRT_SECURE_NO_WARNINGS
#include "SerialPort.h"
#include "Impl/SerialPortImpl.h"
#include "SerialPortVersion.h"



// Link namespaces.
using namespace cr::clib;



std::string SerialPort::getVersion()
{
    return SERIAL_PORT_VERSION;
}



SerialPort::SerialPort() : m_impl(new SerialPortImpl())
{

}



SerialPort::~SerialPort()
{
    delete m_impl;
}



void SerialPort::close()
{
    m_impl->close();

}



int SerialPort::read(uint8_t *buf, uint32_t size)
{
    return m_impl->read(buf, size);
}



int SerialPort::write(uint8_t *buf, uint32_t size)
{
    return m_impl->write(buf, size);
}



bool SerialPort::open(
    std::string comport_file,
    unsigned int baudrate,
    unsigned int timeout,
    const char *mode) {

    return m_impl->open(comport_file, baudrate, timeout, mode);
}



bool SerialPort::isOpen()
{
    return m_impl->isOpen();
}



bool SerialPort::setFlowControl(bool enable)
{
    return m_impl->setFlowControl(enable);
}