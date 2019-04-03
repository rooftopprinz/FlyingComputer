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

**LoRa Packet Definition**

| Name | Description | Size |
|------|-------------|------|
| TYPE	| Message Id		| 1 |
| SN	| Sequence Number	| 1 |
| MAC	| Message Integrity	| 2 |
| DATA	| Data Payload		| N |

**Payload Format**

| ID	| Name	| Size |
|-------|-------|------|
| UID	| Param Identifier	| 1 |
| SIZ	| Proceeding Size	| 1 |
| DAT	| Data |SIZ |

SIZ and DAT are always pair.

**Message Definitions**

| Message Type	| Message Name	| Data Payload |
|---------------|---------------|--------------|
| 0	|Read Group Request	| UID
| 1	| Read Group Response	| DAT
| 2	| Write Group Request	| UID \| SIZ \| DAT
| 3	| Write Group Response	
| 4	| Write Group Indication	| UID \| SIZ \| DAT
| 5	| Read Data Request	| UID
| 6	| Read Data Response	| SIZ \| DAT
| 7	| Write Data Request	| UID \| SIZ \| DAT
| 8	| Write Data Response	
| 9	| Write Data Indication	| UID \| SIZ \| DAT

## FlyItf GND
Interfaces FlyDb to user input / output display.<br />
Schedules periodic uplink data from user input and setting.

## FlyComm
see https://github.com/therooftopprinz/PiLoRa


## FlyPilot
Advance auto pilot and fly by wire.

![alt text](https://raw.githubusercontent.com/therooftopprinz/FlyingComputer/master/Architecture/fcapcs.png "FlyAdvance")
