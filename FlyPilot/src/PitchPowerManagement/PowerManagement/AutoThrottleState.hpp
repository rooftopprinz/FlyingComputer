#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_AUTOTHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_AUTOTHROTTLESTATE_HPP_

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <src/StateMachine/StateMachine.hpp>
#include "CommonThrottleState.hpp"

class CommonThrottleState;
class ManualThrottleState;
class AutoThrottleState;
class TogaLkThrottleState;
class IFlightInstrumentContext;

class AutoThrottleState : public CommonThrottleState
{
public:
    AutoThrottleState(IFiniteStateMachine& fsm, IFlightInstrumentContext& flightInstrumentContext);
    ~AutoThrottleState();
    void onEnter();
    void onExit();
    void onEvent(FdChangeEvent& event);
private:
    void controlLoop();

    bool pauseControlLoop;
    std::atomic<bool> exitControlLoop;
    std::mutex pauseControlLoopMutex;
    std::condition_variable pauseControlLoopCv;
    std::thread controlLoopThread;
};

#endif