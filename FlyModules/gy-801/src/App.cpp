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
    : mChannel(pArgs.getChannel())
    , mCtrlAddr(pArgs.getCtrlAddr())
    , mMode(pArgs.isTx()? Mode::TX : Mode::RX)
    , mIoAddr(pArgs.getIoAddr())
    , mCarrier(pArgs.getCarrier())
    , mBw(pArgs.getBw())
    , mCr(pArgs.getCr())
    , mSf(pArgs.getSf())
    , mMtu(pArgs.getMtu())
    , mTxPower(pArgs.getTxPower())
    , mRxGain(pArgs.getLnaGain())
    , mResetPin(pArgs.getResetPin())
    , mDio1Pin(pArgs.getGetDio1Pin())
    , mCtrlSock(pUdpFactory.create())
    , mIoSock(pUdpFactory.create())
    , mSpi(hwapi::getSpi(mChannel))
    , mGpio(hwapi::getGpio())
    , mModule(*mSpi, *mGpio, mResetPin, mDio1Pin)
    , mLogger("App")
{
    mLogger << logger::DEBUG << "-------------- Parameters ---------------";
    mLogger << logger::DEBUG << "channel:         " << mChannel;
    mLogger << logger::DEBUG << "Mode:            " << ((const char*[]){"TX", "RX"})[int(mMode)];
    mLogger << logger::DEBUG << "Control Address: "
        << ((mCtrlAddr.addr>>24)&0xFF) << "."
        << ((mCtrlAddr.addr>>16)&0xFF) << "."
        << ((mCtrlAddr.addr>>8)&0xFF) << "."
        << (mCtrlAddr.addr&0xFF) << ":"
        << (mCtrlAddr.port);
    mLogger << logger::DEBUG << "TX/RX Address:   "
        << ((mIoAddr.addr>>24)&0xFF) << "."
        << ((mIoAddr.addr>>16)&0xFF) << "."
        << ((mIoAddr.addr>>8)&0xFF) << "."
        << (mIoAddr.addr&0xFF) << ":"
        << (mIoAddr.port);
    mLogger << logger::DEBUG << "Carrier:         " << mCarrier << " Hz";
    mLogger << logger::DEBUG << "bandwidth:       " <<
        ((const char*[]){"7.8", "10.4", "15.6", "20.8", "31.25", "41.7", "62.5", "125", "250", "500",})[int(mBw)] << " kHz";
    mLogger << logger::DEBUG << "Coding Rate:     " <<
        ((const char*[]){0, "4/5", "4/6", "4/7", "4/8"})[int(mCr)];
    mLogger << logger::DEBUG << "Spread Factor:   " <<
        ((const char*[]){0,0,0,0,0,0,"SF6", "SF7", "SF8", "SF9", "SF10", "SF11", "SF12"})[int(mSf)];
    mLogger << logger::DEBUG << "MTU:             " << mMtu;
    mLogger << logger::DEBUG << "Tx Power:        " << mTxPower;
    mLogger << logger::DEBUG << "Rx Gain:         " <<
        ((const char*[]){"", "G1", "G2", "G3", "G4", "G5", "G6"})[int(mRxGain)];
    mLogger << logger::DEBUG << "Reset Pin:       " << mResetPin;
    mLogger << logger::DEBUG << "TX/RX Done Pin:  " << mDio1Pin;

    mCtrlSock->bind(mCtrlAddr);
    if (Mode::TX == mMode)
    {
        mIoSock->bind(mIoAddr);
    }
    else
    {
        mIoSock->bind({});
    }
}

int App::run()
{
}


} // namespace app