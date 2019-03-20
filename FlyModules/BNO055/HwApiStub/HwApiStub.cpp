#include <sstream>
#include <iomanip>
#include <cstring>
#include <map>
#include <memory>
#include <thread>
#include <chrono>
#include <IHwApi.hpp>
#include <Logger.hpp>
#include <Udp.hpp>

namespace hwapi
{

struct GpioStub : IGpio
{
public:
    int setMode(unsigned pGpio, PinMode pMode)
    {
        // mLogger << logger::DEBUG << " setMode(" << pGpio << ", " << (PinMode::OUTPUT==pMode? "OUTPUT" : "INPUT") << ")" ;
        return 0;
    }

    int get(unsigned pGpio)
    {
        // mLogger << logger::DEBUG << " get(" << pGpio << ")";
        return 0;
    }

    int set(unsigned pGpio, unsigned pLevel)
    {
        // mLogger << logger::DEBUG << " set(" << pGpio << ", " << pLevel << ")";
        return 0;
    }

    int registerCallback(unsigned pUserGpio, Edge pEdge, std::function<void(uint32_t tick)> pCb)
    {
        cb = pCb;
        // mLogger << logger::DEBUG << " registerCallback(" << pUserGpio << ", " << (Edge::FALLING==pEdge? "FALLING" : "RISING") << ", cb)" ;
        return 0;
    }

    int deregisterCallback(int pCallbackId)
    {
        // mLogger << logger::DEBUG << " deregisterCallback(" << pCallbackId << ")" ;
        return 0;
    }

    std::function<void(uint32_t tick)> cb;
private:
    // logger::Logger mLogger = logger::Logger("Gpio");
};

std::shared_ptr<ISpi> getSpi(uint8_t pChannel);
std::shared_ptr<IGpio> getGpio();

std::shared_ptr<ISpi> getSpi(uint8_t pChannel)
{
    static std::map<int, std::shared_ptr<ISpi>> spiMap;
    auto it = spiMap.find(pChannel);
    if (spiMap.end() == it)
    {
        // spiMap.emplace(pChannel, std::make_shared<Sx1278SpiStub>(pChannel));
    }
    return spiMap[pChannel];
}

std::shared_ptr<IGpio> getGpio()
{
    static std::shared_ptr<IGpio> gpio = std::make_shared<GpioStub>();
    return gpio;
}

void setup()
{
}

void teardown()
{
}

} // hwapi