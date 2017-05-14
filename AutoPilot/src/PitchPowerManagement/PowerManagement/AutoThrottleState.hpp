#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_AUTOTHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_AUTOTHROTTLESTATE_HPP_

#include <thread>
#include <mutex>
#include <src/PitchPowerManagement/IPitchPowerContext.hpp>
#include <src/FlightCalculator/IFlightContext.hpp>
#include <src/PitchPowerManagement/IPitchPowerContext.hpp>
#include <src/StateMachine/StateMachine.hpp>
#include "ThrottleEventHandler.hpp"
#include "ManualThrottleState.hpp"
#include "TogaThrottleState.hpp"
#include "TogaLkThrottleState.hpp"
#include "IdleThrottleState.hpp"

class ManualThrottleState;
class TogaThrottleState;
class TogaLkThrottleState;
class IdleThrottleState;


class AutoThrottleState : public IState, public ThrottleEventHandler
{
public:
    AutoThrottleState(IFiniteStateMachine& fsm,
        IPitchPowerContext& pitchPowerContext,
        IFlightContext& flightContext);
    ~AutoThrottleState();
    void onEnter();
    void onExit();
    void onEvent(SpeedChangeEvent& event);
    void onEvent(LeverChangeEvent& event);
    void onEvent(FdChangeEvent& event);
    void onEvent(PowerModeChangeEvent& event);
    void onEvent(EffectiveStallSpeedChangeEvent& event);
    void setTargetStateInstances(ManualThrottleState& manualThrottleState, TogaThrottleState& togaThrottleState,
        TogaLkThrottleState& togaLkThrottleState, IdleThrottleState& idleThrottleState);
private:
    void controlLoop();

    IFiniteStateMachine& fsm;
    IPitchPowerContext& pitchPowerContext;
    IFlightContext& flightContext;
    ManualThrottleState* manualThrottleState;
    TogaThrottleState* togaThrottleState;
    TogaLkThrottleState* togaLkThrottleState;
    IdleThrottleState* idleThrottleState;

    bool pauseControlLoop;
    bool exitControlLoop;
    std::mutex pauseControlLoopMutex;
    std::thread controlLoopThread;
};

#endif