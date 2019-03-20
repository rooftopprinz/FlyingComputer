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

    net::IpPort mCtrlAddr;
    std::unique_ptr<net::ISocket> mCtrlSock;
    std::thread mSensorLoop;

    std::shared_ptr<hwapi::II2C> mBno055I2C;
    bno055::BNO055 mBno055;
};

}
#endif //__APP_HPP__