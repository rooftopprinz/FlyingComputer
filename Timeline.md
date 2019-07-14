* To be able to provide communication between FlyingComputer components by providing interface to access to the key value database.
  * To be able for server to handle client write to the database. **(CW27)** **DONE**
  * To be able for server to handle client read to the database. **(CW27)** **DONE**
  * To be able for client to write to the database. **(CW28)** **DONE**
  * To be able for client to read from the database. **(CW28)** **DONE**

- FlyDB
  - Protocol Message Handler 
    - ReadRequest - Server **DONE**
    - ReadResponse - Client **DONE**
    - WriteRequest - Server **DONE**
    - WriteResponse - Client **DONE**
    - ErrorIndication - Server **NOT NEEDED**
  - Server App **DONE**
  - Client Library **DONE**

* To be able to provide magnetic orientation, absolute orientation and linear acceleration to the FlyingComputer components.
  * To be able to get magnetic orientation from device module. **(CW29)**
  * To be able to get absolute orientation from device module. **(CW29)**
  * To be able to get linear acceleration from device module. **(CW29)**
  * To be able to write device module readings to the database. **(CW29)**

- IMU System
  - Module driver **(CW29)**
    - Register Table **DONE**
    - Base configuration **(CW29)**
    - IO **(CW29)**
  - App

- PWM System
  - Module driver
    - Register Table
    - Configuration
    - IO
  - App

- BAROTEMP System
  - Module Driver
    - Register Table
    - Configure
    - IO
  - App

- Nano System
  - Module driver
    - Protocol Definition
    - Master Handler
    - Slave Handler
  - GPS Module Driver
    - Register Table
    - Configuration
    - IO
  - Analog Acquisition
  - Master App
  - Slave App

- FlyBasic
  - Basic Mapper
  - App

- FlyGround
  - Joystick Driver
  - Instrument IO
  - App
  
