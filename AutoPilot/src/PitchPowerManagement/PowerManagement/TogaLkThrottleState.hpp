#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_TOGALKTHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_TOGALKTHROTTLESTATE_HPP_

#include <src/StateMachine/StateMachine.hpp>
#include "ThrottleEventHandler.hpp"

class TogaLkThrottleState : public IState, public ThrottleEventHandler
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