#include <memory>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <src/PitchPowerManagement/PowerManagement/ManualThrottleState.hpp>
#include <src/PitchPowerManagement/PowerManagement/ThrottleFsm.hpp>

#include <tests/mock/FlightContextMock.hpp>
#include <tests/mock/PitchPowerContextMock.hpp>
#include <tests/mock/FiniteStateMachineMock.hpp>

using namespace testing;

struct ManualThrottleStateTests : public ::testing::Test
{

    ManualThrottleStateTests():
        manState(fsmMock, ppctxMock, fltctxMock)
    {
        manState.setTargetStateInstances(atState, tgState, tglState, idlState);
    }

    FiniteStateMachineMock fsmMock;
    PitchPowerContextMock ppctxMock;
    FlightContextMock  fltctxMock;

    AutoThrottleState atState;
    TogaThrottleState tgState;
    TogaLkThrottleState tglState;
    IdleThrottleState idlState;
    ManualThrottleState manState;
};

TEST_F(ManualThrottleStateTests, shouldTransitToAthrWhenModeSelected)
{
    PowerModeChangeEvent pmch = {EPowerMode::SELECTED};
    EXPECT_CALL(fsmMock, changeState(Ref(atState)));
    manState.onEvent(pmch);
}

TEST_F(ManualThrottleStateTests, shouldTransitToAthrWhenModeManaged)
{
    PowerModeChangeEvent pmch = {EPowerMode::MANAGED};
    EXPECT_CALL(fsmMock, changeState(Ref(atState)));
    manState.onEvent(pmch);
}

TEST_F(ManualThrottleStateTests, shouldTransitToga)
{
    LeverChangeEvent lvrCh = {1.0};
    EXPECT_CALL(fsmMock, changeState(Ref(tgState)));
    manState.onEvent(lvrCh);
}

TEST_F(ManualThrottleStateTests, shouldTransitTogaLkWhenAirspeedSpeedLowers)
{
    
}

TEST_F(ManualThrottleStateTests, shouldTransitTogaLkWhenStallSpeedHighers)
{
    
}

TEST_F(ManualThrottleStateTests, shouldTransitIdle)
{
    LeverChangeEvent lvrCh = {1.0};
    EXPECT_CALL(fsmMock, changeState(Ref(tgState)));
    manState.onEvent(lvrCh);

}