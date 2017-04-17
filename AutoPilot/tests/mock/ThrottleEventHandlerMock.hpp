#ifndef AUTOPILOT_TEST_MOCK_IPITCHPOWERCONTEXTMOCK_HPP_
#define AUTOPILOT_TEST_MOCK_IPITCHPOWERCONTEXTMOCK_HPP_

#include <gmock/gmock.h>

#include <src/PitchPowerManagement/PowerManagement/IThrottleEventHandler.hpp>

struct ThrottleEventHandlerMock : public IThrottleEventHandler
{
    MOCK_METHOD1(onEvent, void(SpeedChangeEvent& event));
    MOCK_METHOD1(onEvent, void(LeverChangeEvent& event));
    MOCK_METHOD1(onEvent, void(FdChangeEvent& event));
    MOCK_METHOD1(onEvent, void(PowerModeChangeEvent& event));
    MOCK_METHOD1(onEvent, void(VrsChangeEvent& event));
};

#endif