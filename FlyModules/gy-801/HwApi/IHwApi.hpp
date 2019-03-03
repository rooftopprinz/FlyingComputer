#ifndef __HWAPI_IHWAPI_HPP__
#define __HWAPI_IHWAPI_HPP__

#include <functional>
#include <memory>

namespace hwapi
{

struct ISpi
{
    virtual int read(uint8_t *data, unsigned count) = 0;
    virtual int write(uint8_t *data, unsigned count) = 0;
    virtual int xfer(uint8_t *dataOut, uint8_t *dataIn, unsigned count) = 0;
};


struct II2C
{
    virtual int writBlock(unsigned reg, uint8_t* buf, unsigned count) = 0;
    virtual int readBlock(unsigned reg, uint8_t* buf, unsigned count) = 0;
};

enum class PinMode
{
    OUTPUT,
    INPUT
};

enum class Edge
{
    FALLING,
    RISING
};

struct IGpio
{
    virtual int setMode(unsigned pGpio, PinMode pMode) = 0;
    virtual int get(unsigned pGpio) = 0;
    virtual int set(unsigned pGpio, unsigned pLevel) = 0;
    virtual int registerCallback(unsigned pUserGpio, Edge pEdge, std::function<void(uint32_t tick)> pCb) = 0;
    virtual int deregisterCallback(int pCallbackId) = 0;
};

std::shared_ptr<ISpi>  getSpi(uint8_t channel);
std::shared_ptr<IGpio> getGpio();
void setup();
void teardown();

} // hwapi

#endif // __HWAPI_IHWAPI_HPP__
