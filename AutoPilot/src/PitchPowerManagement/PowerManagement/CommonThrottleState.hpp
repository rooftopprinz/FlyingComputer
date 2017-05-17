#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_COMMONTHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_COMMONTHROTTLESTATE_HPP_

#include <src/StateMachine/StateMachine.hpp>
#include "IThrottleEventHandler.hpp"

class StateMachine;
class CommonThrottleState;
class ManualThrottleState;
class AutoThrottleState;
class TogaLkThrottleState;
class IFlightInstrumentContext;

class CommonThrottleState : public IState, public IThrottleEventHandler
{
public:
    CommonThrottleState(IFiniteStateMachine& fsm,
        IFlightInstrumentContext& flightInstrumentContext);

    ~CommonThrottleState();

    void onEvent(SpeedChangeEvent& event);
    void onEvent(LeverChangeEvent& event);
    void onEvent(FdChangeEvent& event);
    void onEvent(PowerModeChangeEvent& event);
    void onEvent(EffectiveStallSpeedChangeEvent& event);
    void setTargetStateInstances(ManualThrottleState& manualThrottleState, AutoThrottleState& autoThrottleState,
        TogaLkThrottleState& togaLkThrottleState);

protected:
    IFiniteStateMachine& fsm;
    IFlightInstrumentContext& flightInstrumentContext;
    ManualThrottleState* manualThrottleState;
    AutoThrottleState* autoThrottleState;
    TogaLkThrottleState* togaLkThrottleState;
};

#endif