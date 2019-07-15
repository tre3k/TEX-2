cmake_minimum_required(VERSION 3.10)

project(Pneumo)

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


add_executable(Pneumo
        main.cpp
        ClassFactory.cpp
        PneumoStateMachine.cpp
        Pneumo.cpp Pneumo.h
        PneumoClass.cpp PneumoClass.h
        SerialPort.cpp SerialPort.h
        Pneumatics.cpp Pneumatics.h
        ControllerRA153_16.cpp ControllerRA153_16.h)

target_link_libraries(Pneumo
        ${TANGO_LIB}
        ${OMNIDYNAMIC4_LIB}
        ${COS4_LIB}
        ${OMNIORB4_LIB}
        ${OMNITHREAD_LIB}
        ${LOG4TANGO_LIB}
        ${ZMQ_LIB}
        ${BOOST_SYSTEM_LIB})

