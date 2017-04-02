#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_AUTOTHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_AUTOTHROTTLESTATE_HPP_

#include <src/StateMachine/StateMachine.hpp>
#include "ThrottleEventHandler.hpp"
#include "PowerEvents.hpp"

class AutoThrottleState : public IState, public ThrottleEventHandler
{
public:
    void onEnter() override;
    void onExit() override;
    void onEvent(SpeedChangeEvent& event) override;
    void onEvent(LeverChangeEvent& event) override;
    void onEvent(FdChangeEvent& event) override;
    void onEvent(PowerModeChangeEvent& event) override;
    void onEvent(VrsChangeEvent& event) override;
private:
};

#endif