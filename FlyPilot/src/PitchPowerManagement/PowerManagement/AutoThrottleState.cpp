#include "AutoThrottleState.hpp"
#include <chrono>
AutoThrottleState::AutoThrottleState(IFiniteStateMachine& fsm, IFlightInstrumentContext& flightInstrumentContext):
    CommonThrottleState(fsm, flightInstrumentContext),
    pauseControlLoop(true),
    exitControlLoop(false),
    controlLoopThread(&AutoThrottleState::controlLoop, this)
{
}

AutoThrottleState::~AutoThrottleState()
{
    exitControlLoop.store(true);
    pauseControlLoopCv.notify_all();
    controlLoopThread.join();
}


void AutoThrottleState::onEnter()
{
    {
        std::lock_guard<std::mutex> pauseLock(pauseControlLoopMutex);
        pauseControlLoop = false;
    }
    pauseControlLoopCv.notify_all();
}

void AutoThrottleState::onExit()
{
    {
        std::lock_guard<std::mutex> pauseLock(pauseControlLoopMutex);
        pauseControlLoop = true;
    }
}

void AutoThrottleState::onEvent(FdChangeEvent& event)
{
    // TODO: Differentiate from MANAGED and SELECTED mode.
}

void AutoThrottleState::controlLoop()
{
    while (true)
    {
        // TODO: Do computations here

        std::unique_lock<std::mutex> pauseLock(pauseControlLoopMutex);
        pauseControlLoopCv.wait(pauseLock, [this](){
            return !pauseControlLoop || exitControlLoop.load();
        });
        if (exitControlLoop.load())
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // TODO: Fetch refresh rate on config
    }
}