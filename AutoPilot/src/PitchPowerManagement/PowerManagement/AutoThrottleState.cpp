#include "AutoThrottleState.hpp"
#include <chrono>
AutoThrottleState::AutoThrottleState(IFiniteStateMachine& fsm, IFlightInstrumentContext& flightInstrumentContext):
    fsm(fsm),
    flightInstrumentContext(flightInstrumentContext),
    pauseControlLoop(true),
    exitControlLoop(false),
    controlLoopThread(&AutoThrottleState::controlLoop, this)
{
}

AutoThrottleState::~AutoThrottleState()
{
    exitControlLoop.store(true);
    pauseControlLoopMutex.notify_all();
    controlLoopThread.join();
}


void AutoThrottleState::onEnter()
{
    {
        std::lock_guard<std::mutex> pauseLock(pauseControlLoopMutex);
        pauseControlLoop = false;
    }
    pauseControlLoopMutex.notify_all();
}

void AutoThrottleState::onExit()
{
    {
        std::lock_guard<std::mutex> pauseLock(pauseControlLoopMutex);
        pauseControlLoop = true;
    }
}

void AutoThrottleState::onEvent(SpeedChangeEvent& event)
{
    if (flightInstrumentContext.getEffectiveStallSpeed() <= event.speed)
    {
        fsm.changeState(*togaLkThrottleState);
    }
}

void AutoThrottleState::onEvent(LeverChangeEvent& event)
{
    if (event.lever == 1.0)
    {
        fsm.changeState(*togaThrottleState);
    }
    else if (event.lever == 1.0)
    {
        fsm.changeState(*idleThrottleState);
    }
}

void AutoThrottleState::onEvent(FdChangeEvent& event)
{
    // TODO: Differentiate from MANAGED and SELECTED mode.
}

void AutoThrottleState::onEvent(PowerModeChangeEvent& event)
{
    if (event.mode == EPowerMode::MANUAL)
    {
        fsm.changeState(*manualThrottleState);
    }
}

void AutoThrottleState::onEvent(EffectiveStallSpeedChangeEvent& event)
{
    if (flightInstrumentContext.getIndicatedAirspeed() <= event.speed)
    {
        fsm.changeState(*togaLkThrottleState);
    }
}

void AutoThrottleState::setTargetStateInstances(ManualThrottleState& manualThrottleState, TogaThrottleState& togaThrottleState,
    TogaLkThrottleState& togaLkThrottleState, IdleThrottleState& idleThrottleState)
{
    this->manualThrottleState = &manualThrottleState;
    this->togaThrottleState = &togaThrottleState;
    this->togaLkThrottleState = &togaLkThrottleState;
    this->idleThrottleState = &idleThrottleState;
}

void AutoThrottleState::controlLoop()
{
    while (exitControlLoop.load())
    {

        // TODO: Do computations here

        std::unique_lock<std::mutex> pauseLock(pauseControlLoopMutex);
        pauseControlLoopMutex.wait(pauseLock, [this](){
            return pauseControlLoop || !exitControlLoop.load();
        });
        std::this_thread::sleep_for(std::chrono::millisecond(100)); // TODO: Fetch refresh rate on config
    }
}