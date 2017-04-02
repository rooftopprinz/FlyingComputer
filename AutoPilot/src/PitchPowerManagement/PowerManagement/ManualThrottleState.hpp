#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_MANUALTHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_MANUALTHROTTLESTATE_HPP_

#include <src/PitchPowerManagement/IPitchPowerContext.hpp>
#include <src/StateMachine/StateMachine.hpp>
#include "ThrottleEventHandler.hpp"
#include "AutoThrottleState.hpp"
#include "TogaThrottleState.hpp"
#include "TogaLkThrottleState.hpp"
#include "IdleThrottleState.hpp"

class AutoThrottleState;
class TogaThrottleState;
class TogaLkThrottleState;
class IdleThrottleState;

class ManualThrottleState : public IState, public ThrottleEventHandler
{
public:
    ManualThrottleState(IFiniteStateMachine& fsm, 
        AutoThrottleState& autoThrottleState,
        TogaThrottleState& togaThrottleState,
        TogaLkThrottleState& togaLkThrottleState,
        IdleThrottleState& idleThrottleState);

    ~ManualThrottleState();
    void onEnter() override;
    void onExit() override;
    void onEvent(SpeedChangeEvent& event) override;
    void onEvent(LeverChangeEvent& event) override;
    void onEvent(FdChangeEvent& event) override;
    void onEvent(PowerModeChangeEvent& event) override;
    void onEvent(VrsChangeEvent& event) override;
private:
    IFiniteStateMachine& fsm;
    AutoThrottleState& autoThrottleState;
    TogaThrottleState& togaThrottleState;
    TogaLkThrottleState& togaLkThrottleState;
    IdleThrottleState& idleThrottleState;
};

#endif