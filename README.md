# FlyingComputer

## Architecture
![alt text](https://raw.githubusercontent.com/therooftopprinz/FlyingComputer/master/Architecture/wholesystemv0.png "Architecture")
## FlyBasic
Basic control input to output mapping.
## FlyModules

### GY-801 (cancelled in favor of BNO55)
Driver status
* L3G4200D - **DONE**
* ADXL345 - **Not tested**
* HMC5883L - **CANCELLED**
* BMP180 - **MOVED**

### BNO55
9-DOF IMU with sensor fusion over UART.

### BMP180
Temperature and Pressure sensor over I2C.

### PCA9685
16-channel 16-bit PWM controller over I2C.

### Nano
Collects data from GY-NEO6MV2 over UART2, Differential Pressure sensor over analog and interfaces Raspberry Pi over bb UART.

## FlyItf AIR
Interfaces GND to FlyDb over FlyComm (PiLoRa).<br />
Schedules periodic downlink data from FlyDb.

## FlyItf GND
Interfaces FlyDb to user input / output display.<br />
Schedules periodic uplink data from user input and setting.

## FlyComm
see https://github.com/therooftopprinz/PiLoRa


## FlyPilot
Advance auto pilot and fly by wire.

![alt text](https://raw.githubusercontent.com/therooftopprinz/FlyingComputer/master/Architecture/fcapcs.png "FlyAdvance")
