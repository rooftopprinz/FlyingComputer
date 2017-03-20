#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_AUTOTHROTTLEFSM_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_AUTOTHROTTLEFSM_HPP_

#include <src/StateMachine/StateMachine.hpp>

#include "ManualThrottleState.hpp"
#include "AutoThrottleState.hpp"
#include "TogaThrottleState.hpp"
#include "TogaLkThrottleState.hpp"
#include "IdleThrottleState.hpp"

class ThrottleFsm :  public IFiniteStateMachine
{
public:

    void changeState(IState& target);
    void triggerEvent(Event& event);
private:
    IState *currentThrottleState;
    ManualThrottleState manualThrottleState;
    AutoThrottleState autoThrottleState;
    TogaThrottleState togaThrottleState;
    TogaLkThrottleState togaLkThrottleState;
    IdleThrottleState idleLkThrottleState;
}

#endif