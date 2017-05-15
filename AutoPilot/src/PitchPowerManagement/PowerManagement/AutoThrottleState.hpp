#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_AUTOTHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_AUTOTHROTTLESTATE_HPP_

#include <thread>
#include <mutex>
#include <condition_variable>
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
    AutoThrottleState(IFiniteStateMachine& fsm, IFlightInstrumentContext& flightInstrumentContext);
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
    IFlightInstrumentContext& flightInstrumentContext;
    ManualThrottleState* manualThrottleState;
    TogaThrottleState* togaThrottleState;
    TogaLkThrottleState* togaLkThrottleState;
    IdleThrottleState* idleThrottleState;

    bool pauseControlLoop;
    std::atomic<bool> exitControlLoop;
    std::mutex pauseControlLoopMutex;
    std::condition_variable pauseControlLoopCv;
    std::thread controlLoopThread;
};

#endif