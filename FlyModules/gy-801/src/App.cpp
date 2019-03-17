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
    , mI2CGyro (hwapi::getI2C(0x69))
    , mGyro(*mI2CGyro)
    , mI2CAccel(hwapi::getI2C(0x53))
    , mAccel(*mI2CAccel)
{
    Logless("App::App ------------- Parameters ---------------");
    Logless("App::App control address: _._._._:_",
        ((mCtrlAddr.addr>>24)&0xFF),
        ((mCtrlAddr.addr>>16)&0xFF),
        ((mCtrlAddr.addr>>8)&0xFF),
        (mCtrlAddr.addr&0xFF),
        (mCtrlAddr.port));

    mCtrlSock->bind(mCtrlAddr);
    mGyro.configure(l3g4200d::DataRate::DR_100HZ, l3g4200d::Bandwidth::BW_HIGH);
    mAccel.configure();
    mSensorLoop = std::thread(&App::sensorLoop, this);
}

void App::calibrateGyro()
{
    mGyro.bypass();
    mGyro.fifo();

    {
        std::ifstream istrm("gyrocalibration", std::ios::binary);
        istrm >> mGyroDcOffsetX;
        istrm >> mGyroDcOffsetY;
        istrm >> mGyroDcOffsetZ;
    }

    // return;

    // size_t i=0;
    // while (i<1000)
    // {
    //     using namespace std::literals::chrono_literals;
    //     std::this_thread::sleep_for(250ms); // TODO: Base it on ODR 
    //     const size_t sz = mGyro.readRaw((uint8_t*)&mXYZws);
    //     const uint64_t tnow = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    //     const uint64_t diff = tnow-mGyroLastSampleTime;
    //     const double deltaT = double(diff)/1000000.0/sz;
    //     mGyroLastSampleTime = tnow;

    //     for (size_t j=0; j<sz; j++, i++)
    //     {
    //         Logless("App::calibrateGyro i=_ diff=_ s x=_ y=_ z=_ ", i, deltaT, mXYZws[j].x, mXYZws[j].y, mXYZws[j].z);
    //         mGyroDcOffsetZ += mXYZws[j].z;
    //         mGyroDcOffsetY += mXYZws[j].y;
    //         mGyroDcOffsetX += mXYZws[j].x;
    //     }
    // }

    // mGyroDcOffsetZ /= i;
    // mGyroDcOffsetY /= i;
    // mGyroDcOffsetX /= i;

    Logless("App::calibrateGyro DCBIAS=(_ _ _)",
        mGyroDcOffsetX,
        mGyroDcOffsetY,
        mGyroDcOffsetZ);
}

void App::readGyro()
{
    static double sensorTime = 0.0;
    const size_t sz = mGyro.readRaw((uint8_t*)&mXYZws);
    const uint64_t tnow = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    const uint64_t diff = tnow-mGyroLastSampleTime;
    double deltaT = double(diff)/1000000.0/sz;
    mGyroLastSampleTime = tnow;
    const double scalingFactor = mGyro.getScalingFactor();
    for (size_t i=0; i<sz; i++)
    {
        Logless("App::readGyro[_] diff=_ s t=_ s x=_ y=_ z=_ ", i, deltaT, sensorTime, mXYZws[i].x, mXYZws[i].y, mXYZws[i].z);
        {
            sensorTime += deltaT;
            std::unique_lock<std::mutex> ulock(mXYZlock);   
            mX += ((mXYZws[i].x-mGyroDcOffsetX)*deltaT*scalingFactor);
            mY += ((mXYZws[i].y-mGyroDcOffsetY)*deltaT*scalingFactor);
            mZ += ((mXYZws[i].z-mGyroDcOffsetZ)*deltaT*scalingFactor);
        }
    }
}

void App::readAccel()
{
    static double sensorTime = 0.0;
    const size_t sz = mAccel.readRaw((uint8_t*)&mXYZg);
    const uint64_t tnow = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    const uint64_t diff = tnow-mAccelLastSampleTime;
    double deltaT = double(diff)/1000000.0/sz;
    mAccelLastSampleTime = tnow;
    const double scalingFactor = 0.004; // assume full scale

    float xAccelSum = 0.0;
    float yAccelSum = 0.0;
    float zAccelSum = 0.0;

    for (size_t i=0; i<sz; i++)
    {
        Logless("App::readAccel[_] diff=_ s t=_ s x=_ y=_ z=_ ", i, deltaT, sensorTime, mXYZg[i].x, mXYZg[i].y, mXYZg[i].z);
        sensorTime += deltaT;
        xAccelSum += ((mXYZg[i].z-mAccelDcOffsetX)*scalingFactor);
        yAccelSum += ((mXYZg[i].y-mAccelDcOffsetY)*scalingFactor);
        zAccelSum += ((mXYZg[i].x-mAccelDcOffsetZ)*scalingFactor);
    }

    {
        std::unique_lock<std::mutex> ulock(mXYZAccelLock);
        Logless("App::readAccel[sum] x=_ y=_ z=_ ", xAccelSum, yAccelSum, zAccelSum);
        sensorTime += deltaT;
        mXAccel = xAccelSum/sz;
        mYAccel = yAccelSum/sz;
        mZAccel = zAccelSum/sz;
    }
}

int App::sensorLoop()
{
    mGyroLastSampleTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    mAccelLastSampleTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    calibrateGyro();
    while(1)
    {
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(250ms); // TODO: Base it on ODR
        Logless("App::sensorLoop 250ms");
        readGyro();
        readAccel();
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