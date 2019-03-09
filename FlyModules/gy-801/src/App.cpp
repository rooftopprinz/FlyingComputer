#include <chrono>
#include <App.hpp>

namespace app
{

Args::Args(const Options& pOptions)
    : mOptions(pOptions)
{}

net::IpPort Args::getCtrlAddr() const
{
    return parseIpPort("cx", {0, 2221u});
}

net::IpPort Args::parseIpPort(std::string pKey, net::IpPort pDefault) const
{
    std::regex addressFilter("([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.([0-9]+):([0-9]+)");
    std::smatch match;
    auto it = mOptions.find(pKey);
    net::IpPort rv;
    if (it == mOptions.cend())
    {
        rv = pDefault;
    }
    else
    {
        if (std::regex_match(it->second, match, addressFilter))
        {
            if (match.size() != 6)
            {
                throw std::runtime_error(std::string("invalid address: `") + it->second + "`");
            }
            uint8_t a = std::stoi(match[1].str());
            uint8_t b = std::stoi(match[2].str());
            uint8_t c = std::stoi(match[3].str());
            uint8_t d = std::stoi(match[4].str());
            uint16_t port = std::stoi(match[5].str());
            rv = net::toIpPort(a,b,c,d,port);
        }
        else
        {
            throw std::runtime_error(std::string("invalid address: `") + it->second + "`");
        }
    }
    return rv;
}

App::App(net::IUdpFactory& pUdpFactory, const Args& pArgs)
    : mCtrlAddr(pArgs.getCtrlAddr())
    , mCtrlSock(pUdpFactory.create())
    , mI2CGyro(hwapi::getI2C(0b1101001))
    , mGyro(*mI2CGyro)
{
    Logless("App::App ------------- Parameters ---------------");
    Logless("App::App control address: _._._._:_",
        ((mCtrlAddr.addr>>24)&0xFF),
        ((mCtrlAddr.addr>>16)&0xFF),
        ((mCtrlAddr.addr>>8)&0xFF),
        (mCtrlAddr.addr&0xFF),
        (mCtrlAddr.port));

    mCtrlSock->bind(mCtrlAddr);
    mGyro.configure(l3g4200d::DataRate::DR_800HZ, l3g4200d::Bandwidth::BW_HIGH);
    mGyroLoop = std::thread(&App::gyroLoop, this);
}

int App::gyroLoop()
{
    while(1)
    {
        auto sz = mGyro.read(mXLwss,mXHwss,mYLwss,mYHwss,mZLwss,mZHwss);
        double deltaT = 0.038/sz;
        for (auto i=0; i<sz; i++)
        {
            int16_t x = (mXLwss[i] << 8) | mXHwss[i];
            int16_t y = (mYLwss[i] << 8) | mYHwss[i];
            int16_t z = (mZLwss[i] << 8) | mZHwss[i];
            float oldXws = mXws;
            float oldYws = mYws;
            float oldZws = mZws;
            mXws = x*2000.0/32767;
            mYws = y*2000.0/32767;
            mZws = z*2000.0/32767;
            mX = (mXws-oldXws)*deltaT;
            mY = (mYws-oldYws)*deltaT;
            mZ = (mZws-oldZws)*deltaT;
        }

        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(38ms);
    }
}

int App::run()
{
    common::Buffer recvbuffer(new std::byte[1024], 1024);
    common::Buffer sendbuffer(new std::byte[1024], 1024);
    while (1)
    {
        net::IpPort src;
        mCtrlSock->recvfrom(recvbuffer, src);
        new (sendbuffer.data()+sizeof(float)*0) float(mZ);
        new (sendbuffer.data()+sizeof(float)*1) float(mY);
        new (sendbuffer.data()+sizeof(float)*2) float(mX);
        common::Buffer tosend(sendbuffer.data(), sizeof(float)*3, false);
        mCtrlSock->sendto(tosend, src);
    }
    return 0;
}

} // namespace app