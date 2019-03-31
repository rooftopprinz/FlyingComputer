#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_AUTOTHROTTLEFSM_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_AUTOTHROTTLEFSM_HPP_

#include <src/StateMachine/StateMachine.hpp>
#include "ThrottleEventHandler.hpp"
#include "ManualThrottleState.hpp"
#include "AutoThrottleState.hpp"
#include "TogaLkThrottleState.hpp"

class ThrottleFsm :  public IFiniteStateMachine, public ThrottleEventHandler
{
public:
    void changeState(IState& target);
    void onEvent(SpeedChangeEvent& event) override;
    void onEvent(LeverChangeEvent& event) override;
    void onEvent(FdChangeEvent& event) override;
    void onEvent(PowerModeChangeEvent& event) override;
    void onEvent(EffectiveStallSpeedChangeEvent& event) override;
private:
    IState *currentThrottleState;
    ManualThrottleState manualThrottleState;
    AutoThrottleState autoThrottleState;
    TogaLkThrottleState togaLkThrottleState;
};

#endif