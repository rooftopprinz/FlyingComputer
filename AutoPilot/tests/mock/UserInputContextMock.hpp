#ifndef AUTOPILOT_TEST_MOCK_IUSERINPUTCONTEXTMOCK_HPP_
#define AUTOPILOT_TEST_MOCK_IUSERINPUTCONTEXTMOCK_HPP_

#include <gmock/gmock.h>

#include <src/Contexts/UserInputContextMock.hpp>

class UserInputContextMock : public IUserInputContext
{
public:
    MOCK_METHOD0(getInputElevator, double());
    MOCK_METHOD0(getInputPowerLeft, double());
    MOCK_METHOD0(getInputPowerRight, double());
    MOCK_METHOD0(getAltSel, double());
    MOCK_METHOD0(getSpdSel, double());
    MOCK_METHOD0(getVsSel, double());
};

#endif