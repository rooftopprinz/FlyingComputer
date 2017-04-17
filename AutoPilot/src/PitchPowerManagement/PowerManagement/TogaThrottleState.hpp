#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_TOGATHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_TOGATHROTTLESTATE_HPP_

#include <src/StateMachine/StateMachine.hpp>
#include <src/PitchPowerManagement/IPitchPowerContext.hpp>
#include "ThrottleEventHandler.hpp"
#include "AutoThrottleState.hpp"
#include "TogaThrottleState.hpp"
#include "TogaLkThrottleState.hpp"
#include "TogaLkThrottleState.hpp"

class TogaThrottleState : public IState, public ThrottleEventHandler
{
public:
    void onEnter() {}
    void onExit() {}
    void onEvent(SpeedChangeEvent& event) {}
    void onEvent(LeverChangeEvent& event) {}
    void onEvent(FdChangeEvent& event) {}
    void onEvent(PowerModeChangeEvent& event) {}
    void onEvent(EffectiveStallSpeedChangeEvent& event) {}
private:
};

#endif