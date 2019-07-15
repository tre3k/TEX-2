cmake_minimum_required(VERSION 3.10)

project(RA153.18)

set(CMAKE_CXX_STANDARD 14)

set(TANGO_INCLUDE_DIR "/usr/include/tango")
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${TANGO_INCLUDE_DIR})

find_library(TANGO_LIB tango)
find_library(OMNIDYNAMIC4_LIB omniDynamic4)
find_library(COS4_LIB COS4)
find_library(OMNIORB4_LIB omniORB4)
find_library(OMNITHREAD_LIB omnithread)
find_library(LOG4TANGO_LIB log4tango)
find_library(ZMQ_LIB zmq)
find_library(BOOST_SYSTEM_LIB boost_system)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_CXX_FLAGS -w)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")


add_executable(RA153.18
        main.cpp
        RA153_18.cpp RA153_18.h
        RA153_18Class.cpp RA153_18Class.h
        RA153_18StateMachine.cpp
        ClassFactory.cpp
        SerialPort.cpp SerialPort.h
        MotorClass.cpp MotorClass.h
        )

target_link_libraries(RA153.18
        ${TANGO_LIB}
        ${OMNIDYNAMIC4_LIB}
        ${COS4_LIB}
        ${OMNIORB4_LIB}
        ${OMNITHREAD_LIB}
        ${LOG4TANGO_LIB}
        ${ZMQ_LIB}
        ${BOOST_SYSTEM_LIB})

