#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_COMMONTHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_COMMONTHROTTLESTATE_HPP_

#include <src/StateMachine/StateMachine.hpp>
#include "ThrottleEventHandler.hpp"
#include "AutoThrottleState.hpp"
#include "AutoThrottleState.hpp"
#include "TogaThrottleState.hpp"
#include "TogaLkThrottleState.hpp"
#include "IdleThrottleState.hpp"

class ManualThrottleState;
class AutoThrottleState;
class TogaThrottleState;
class TogaLkThrottleState;
class IdleThrottleState;
class IFlightInstrumentContext;

class CommonThrottleState : public IState, public ThrottleEventHandler
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
        TogaThrottleState& togaThrottleState, TogaLkThrottleState& togaLkThrottleState, IdleThrottleState& idleThrottleState);

private:
    IFiniteStateMachine& fsm;
    IFlightInstrumentContext& flightInstrumentContext;
    ManualThrottleState* manualThrottleState;
    AutoThrottleState* autoThrottleState;
    TogaThrottleState* togaThrottleState;
    TogaLkThrottleState* togaLkThrottleState;
    IdleThrottleState* idleThrottleState;
};

#endif