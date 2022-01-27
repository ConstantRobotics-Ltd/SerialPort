cmake_minimum_required(VERSION 3.13)



###############################################################################
## PROJECT
## name and version
###############################################################################
project(Tests LANGUAGES CXX)



###############################################################################
## CONFIGURATION
## project configuration
###############################################################################
if (${PARENT}_SERIAL_PORT_TESTS)
    SET(${PARENT}_SERIAL_PORT_TESTS_DATA_RECEIVER       ON CACHE BOOL "" FORCE)
    SET(${PARENT}_SERIAL_PORT_TESTS_DATA_SENDER         ON CACHE BOOL "" FORCE)
endif()



###############################################################################
## INCLUDING SUBDIRECTORIES
## Adding subdirectories according to the configuration
###############################################################################
if (${PARENT}_SERIAL_PORT_TESTS_DATA_RECEIVER)
    add_subdirectory(SerialPortDataReceiver)
endif()
if (${PARENT}_SERIAL_PORT_TESTS_DATA_SENDER)
    add_subdirectory(SerialPortDataSender)
endif()
