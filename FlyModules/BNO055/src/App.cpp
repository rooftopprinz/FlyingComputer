#include <chrono>
#include <fstream>
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
    , mBno055I2C(hwapi::getI2C(0x69))
    , mBno055(*mBno055I2C)
{
    Logless("App::App ------------- Parameters ---------------");
    Logless("App::App control address: _._._._:_",
        ((mCtrlAddr.addr>>24)&0xFF),
        ((mCtrlAddr.addr>>16)&0xFF),
        ((mCtrlAddr.addr>>8)&0xFF),
        (mCtrlAddr.addr&0xFF),
        (mCtrlAddr.port));

    mCtrlSock->bind(mCtrlAddr);
    mBno055.configure();
    mSensorLoop = std::thread(&App::sensorLoop, this);
}


int App::sensorLoop()
{
    mGyroLastSampleTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    mAccelLastSampleTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    // calibrateGyro();
    while(1)
    {
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(250ms); // TODO: Base it on ODR
        Logless("App::sensorLoop 250ms");
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

        float oZ;
        float oY;
        float oX;
        float aZ;
        float aY;
        float aX;

        {
            std::unique_lock<std::mutex> ulock(mXYZlock);
            oZ = float(mZ);
            oY = float(mY);
            oX = float(mX);
        }

        {
            std::unique_lock<std::mutex> ulock(mXYZAccelLock);
            aZ = float(mZAccel);
            aY = float(mYAccel);
            aX = float(mXAccel);
        }

        new (sendbuffer.data()+sizeof(float)*0) float(oZ);
        new (sendbuffer.data()+sizeof(float)*1) float(oY);
        new (sendbuffer.data()+sizeof(float)*2) float(oX);
        new (sendbuffer.data()+sizeof(float)*3) float(aZ);
        new (sendbuffer.data()+sizeof(float)*4) float(aY);
        new (sendbuffer.data()+sizeof(float)*5) float(aX);

        Logless("App::run (_,_,_) (_,_,_)", oX, oY, oZ, aX, aY, aZ);
        common::Buffer tosend(sendbuffer.data(), sizeof(float)*6, false);
        mCtrlSock->sendto(tosend, src);
    }
    return 0;
}

} // namespace app