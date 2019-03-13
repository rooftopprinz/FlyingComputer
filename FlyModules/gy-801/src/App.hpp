#ifndef __APP_HPP__
#define __APP_HPP__

#include <thread>
#include <atomic>
#include <mutex>
#include <regex>
#include <L3G4200D.hpp>
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

    net::IpPort mCtrlAddr;
    std::unique_ptr<net::ISocket> mCtrlSock;

    // Gyro
    std::shared_ptr<hwapi::II2C>   mI2CGyro;
    l3g4200d::L3G4200D mGyro;
    std::thread mGyroLoop;
    XYZ mXYZws[32];
    float mZ = 0.0;
    float mY = 0.0;
    float mX = 0.0;
    int64_t mGyroDcOffsetZ = 0;
    int64_t mGyroDcOffsetY = 0;
    int64_t mGyroDcOffsetX = 0;
    std::mutex mXYZlock;
    uint64_t mGyroLastSampleTime = 0;
};

}
#endif //__APP_HPP__