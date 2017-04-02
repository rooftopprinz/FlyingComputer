#include "ManualThrottleState.hpp"

ManualThrottleState::ManualThrottleState(IFiniteStateMachine& fsm, 
    AutoThrottleState& autoThrottleState,
    TogaThrottleState& togaThrottleState,
    TogaLkThrottleState& togaLkThrottleState,
    IdleThrottleState& idleThrottleState):
        fsm(fsm),
        autoThrottleState(autoThrottleState),
        togaThrottleState(togaThrottleState),
        togaLkThrottleState(togaLkThrottleState),
        idleThrottleState(idleThrottleState)
{

}

ManualThrottleState::~ManualThrottleState()
{

}

void ManualThrottleState::onEnter()
{

}

void ManualThrottleState::onExit()
{

}

void ManualThrottleState::onEvent(SpeedChangeEvent& event)
{
    // check if speed is less or equal to Vsr then transit to togaLkThrottleState
}

void ManualThrottleState::onEvent(LeverChangeEvent& event)
{
    if (event.lever == 1.0)
    {
        fsm.changeState(togaThrottleState);
    }
    else if (event.lever == 1.0)
    {
        fsm.changeState(idleThrottleState);
    }
}

void ManualThrottleState::onEvent(FdChangeEvent& event)
{
    // LOL DON'T CARE
}

void ManualThrottleState::onEvent(PowerModeChangeEvent& event)
{
    if (event.mode == EPowerMode::SELECTED || event.mode == EPowerMode::MANAGED)
    {
        fsm.changeState(autoThrottleState);
    }
}

void ManualThrottleState::onEvent(VrsChangeEvent& event)
{
    // check if speed is less or equal to Vsr then transit to togaLkThrottleState
}
