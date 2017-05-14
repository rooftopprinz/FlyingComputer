#include "AutoThrottleState.hpp"

AutoThrottleState::AutoThrottleState(IFiniteStateMachine& fsm):
        fsm(fsm),
        pauseControlLoop(true),
        exitControlLoop(false),
        controlLoopThread(&AutoThrottleState::controlLoop, this)
{
}

AutoThrottleState::~AutoThrottleState()
{
    controlLoopThread.join();
}


void AutoThrottleState::onEnter()
{

}

void AutoThrottleState::onExit()
{

}

void AutoThrottleState::onEvent(SpeedChangeEvent& event)
{

}

void AutoThrottleState::onEvent(LeverChangeEvent& event)
{

}

void AutoThrottleState::onEvent(FdChangeEvent& event)
{

}

void AutoThrottleState::onEvent(PowerModeChangeEvent& event)
{

}

void AutoThrottleState::onEvent(EffectiveStallSpeedChangeEvent& event)
{

}

void AutoThrottleState::setTargetStateInstances(ManualThrottleState& manualThrottleState, TogaThrottleState& togaThrottleState,
    TogaLkThrottleState& togaLkThrottleState, IdleThrottleState& idleThrottleState)
{

}

void AutoThrottleState::controlLoop()
{

}