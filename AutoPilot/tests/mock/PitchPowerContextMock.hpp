#ifndef AUTOPILOT_TEST_MOCK_IPITCHPOWERCONTEXTMOCK_HPP_
#define AUTOPILOT_TEST_MOCK_IPITCHPOWERCONTEXTMOCK_HPP_

#include <gmock/gmock.h>

#include <src/PitchPowerManagement/IPitchPowerContext.hpp>

struct PitchPowerContextMock : public IPitchPowerContext
{
    MOCK_METHOD1(setOutputPowerLeft, void(double power));
    MOCK_METHOD1(setOutputPowerRight, void(double power));
    MOCK_METHOD1(setOutputElevator, void(double angle));
    MOCK_METHOD0(getInputElevator, double());
    MOCK_METHOD0(getInputPowerLeft, double());
    MOCK_METHOD0(getInputPowerRight, double());
    MOCK_METHOD0(getAltSel, double());
    MOCK_METHOD0(getSpdSel, double());
    MOCK_METHOD0(getVsSel, double());
};

#endif