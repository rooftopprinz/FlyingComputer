#ifndef __TEST_IHWAPIMOCK_HPP__
#define __TEST_IHWAPIMOCK_HPP__

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <IHwApi.hpp>

struct SpiMock : hwapi::ISpi
{
    MOCK_METHOD2(read, int(uint8_t *data, unsigned count));
    MOCK_METHOD2(write, int(uint8_t *data, unsigned count));
    MOCK_METHOD3(xfer, int(uint8_t *dataOut, uint8_t *dataIn, unsigned count));
};

struct GpioMock : hwapi::IGpio
{
    MOCK_METHOD2(setMode, int(unsigned pGpio, hwapi::PinMode pMode));
    MOCK_METHOD1(get, int(unsigned pGpio));
    MOCK_METHOD2(set, int(unsigned pGpio, unsigned pLevel));
    MOCK_METHOD3(registerCallback, int(unsigned pUserGpio, hwapi::Edge pEdge, std::function<void(uint32_t tick)> pCb));
    MOCK_METHOD1(deregisterCallback, int(int));
};

#endif // __TEST_IHWAPIMOCK_HPP__
