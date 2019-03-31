#ifndef AUTOPILOT_TEST_MOCK_IFLIGHTPOWERCONTEXTMOCK_HPP_
#define AUTOPILOT_TEST_MOCK_IFLIGHTPOWERCONTEXTMOCK_HPP_

#include <gmock/gmock.h>

#include <src/Contexts/IPitchPowerContext.hpp>


struct PitchPowerContextMock : public IPitchPowerContext
{
    MOCK_METHOD1(setOutputPowerLeft, void (double power));
    MOCK_METHOD1(setOutputPowerRight, void (double power));
    MOCK_METHOD1(setOutputPower, void (double power));
};

#endif