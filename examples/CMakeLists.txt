cmake_minimum_required(VERSION 3.13)



###############################################################################
## PROJECT
## name and version
###############################################################################
project(Examples LANGUAGES CXX)



###############################################################################
## INCLUDING SUBDIRECTORIES
## Adding subdirectories according to the configuration
###############################################################################
add_subdirectory(SerialPortDataReceiver)
add_subdirectory(SerialPortDataSender)
