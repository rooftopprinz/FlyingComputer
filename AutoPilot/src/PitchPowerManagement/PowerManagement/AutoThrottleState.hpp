#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_AUTOTHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_AUTOTHROTTLESTATE_HPP_

#include <src/StateMachine/StateMachine.hpp>
#include "ThrottleEventHandler.hpp"
#include "PowerEvents.hpp"

class AutoThrottleState : public IState, public ThrottleEventHandler
{
public:
    void onEnter() {}
    void onExit() {}
    void onEvent(SpeedChangeEvent& event) {}
    void onEvent(LeverChangeEvent& event) {}
    void onEvent(FdChangeEvent& event) {}
    void onEvent(PowerModeChangeEvent& event) {}
    void onEvent(EffectiveStallSpeedChangeEvent& event) {}
};

#endif