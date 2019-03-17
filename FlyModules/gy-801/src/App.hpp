#ifndef __APP_HPP__
#define __APP_HPP__

#include <thread>
#include <atomic>
#include <mutex>
#include <regex>
#include <L3G4200D.hpp>
#include <ADXL345.hpp>
#include <IHwApi.hpp>
#include <Logger.hpp>
#include <Udp.hpp>

namespace app
{

using Options = std::map<std::string, std::string>;

class Args
{
public:
    Args(const Options& pOptions);
    net::IpPort getCtrlAddr() const;

private:
    net::IpPort parseIpPort(std::string pKey, net::IpPort pDefault) const;
    const Options& mOptions;
};

struct XYZ
{
    int16_t x;
    int16_t y;
    int16_t z;
};

class App
{
public:
    App(net::IUdpFactory& pUdpFactory, const Args& pArgs);
    int run();

private:
    int sensorLoop();
    void calibrateGyro();
    void readGyro();
    void readAccel();

    net::IpPort mCtrlAddr;
    std::unique_ptr<net::ISocket> mCtrlSock;
    std::thread mSensorLoop;


    // Gyro
    std::shared_ptr<hwapi::II2C>   mI2CGyro;
    l3g4200d::L3G4200D mGyro;
    uint64_t mGyroLastSampleTime = 0;
    XYZ mXYZws[32];
    double mGyroDcOffsetZ = 0;
    double mGyroDcOffsetY = 0;
    double mGyroDcOffsetX = 0;

    // Orientation
    std::mutex mXYZlock;
    float mZ = 0.0;
    float mY = 0.0;
    float mX = 0.0;
    //
    std::mutex mXYZAccelLock;
    float mZAccel = 0.0;
    float mYAccel = 0.0;
    float mXAccel = 0.0;

    // Accel
    std::shared_ptr<hwapi::II2C> mI2CAccel;
    adxl345::ADXL345 mAccel;
    uint64_t mAccelLastSampleTime = 0;
    XYZ mXYZg[32];
    double mAccelDcOffsetZ = 0;
    double mAccelDcOffsetY = 0;
    double mAccelDcOffsetX = 0;
};

}
#endif //__APP_HPP__