cmake_minimum_required(VERSION 3.5)


##
## PROJECT
## name and version
##
project(SerialPortDataSender LANGUAGES CXX)


##
## CONFIGURATION
##
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# creating output directory architecture in accordance with GNU guidelines
set(BINARY_DIR "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BINARY_DIR}/lib")


# create glob files for *.h, *.cpp
file (GLOB H_FILES   ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file (GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
# concatenate the results (glob files) to variable
set  (SOURCES ${CPP_FILES} ${H_FILES})


##
## TARGET
## create target and add include path
##
if (NOT TARGET ${PROJECT_NAME})

    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

if (NOT WIN32)
    target_link_libraries(${PROJECT_NAME} pthread)
endif(NOT WIN32)


target_link_libraries(${PROJECT_NAME} SerialPort)
