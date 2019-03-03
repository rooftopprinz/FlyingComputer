#ifndef __APP_HPP__
#define __APP_HPP__

#include <regex>
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

class App
{
public:
    App(net::IUdpFactory& pUdpFactory, const Args& pArgs);
    int run();

private:
    std::unique_ptr<net::ISocket> mCtrlSock;
    std::unique_ptr<net::ISocket> mIoSock;
    std::shared_ptr<hwapi::ISpi>  mSpi;
    std::shared_ptr<hwapi::I2C>   mI2C;
    std::shared_ptr<hwapi::IGpio> mGpio;
};

}
#endif //__APP_HPP__