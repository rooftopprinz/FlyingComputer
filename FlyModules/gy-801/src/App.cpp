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
{
    Logless("App::App ------------- Parameters ---------------");
    Logless("App::App control address: _._._._:_",
        ((mCtrlAddr.addr>>24)&0xFF),
        ((mCtrlAddr.addr>>16)&0xFF),
        ((mCtrlAddr.addr>>8)&0xFF),
        (mCtrlAddr.addr&0xFF),
        (mCtrlAddr.port));

    mCtrlSock->bind(mCtrlAddr);
}

int App::run()
{
    return 0;
}

} // namespace app