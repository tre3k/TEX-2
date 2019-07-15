# Penumo
## Intro
This soft is the Tango-CS server for custom PNPI device. For control valves pneumo system for moving part of TEX-2

## Properties:
1. SerialPort - just path to serial port (for example "/dev/ttyS0")
2. DeviceAddr - for this device "085" (string format)
3. NumOfValve - number of valve from 0 to 16.

## Attributes:
1. valve, type: bool, only write - for changing state of valve (on/off)
2. sensor, type: bool, only read - read state of pressure sensor, if him value is false, then valve is lock.

## Install:
1. git clone https://github.com/tre3k/Pneumo
2. cd Pneumo
3. mkdir build && cd build
4. cmake ../ 
5. make -j4

