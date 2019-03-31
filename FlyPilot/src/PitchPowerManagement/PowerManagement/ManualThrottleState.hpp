#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_MANUALTHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_MANUALTHROTTLESTATE_HPP_

#include <src/StateMachine/StateMachine.hpp>
#include "CommonThrottleState.hpp"


class AutoThrottleState;
class TogaLkThrottleState;
class IFlightInstrumentContext;

class ManualThrottleState : public CommonThrottleState
{
public:
    ManualThrottleState(IFiniteStateMachine& fsm,
        IFlightInstrumentContext& flightInstrumentContext);

    ~ManualThrottleState();
    void onEnter();
    void onExit();
};

#endif