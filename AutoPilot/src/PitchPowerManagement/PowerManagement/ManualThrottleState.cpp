#include "ManualThrottleState.hpp"
#include <src/Contexts/IFlightInstrumentContext.hpp>

ManualThrottleState::ManualThrottleState(IFiniteStateMachine& fsm,
    IFlightInstrumentContext& flightInstrumentContext):
        fsm(fsm),
        flightInstrumentContext(flightInstrumentContext)
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
    if (flightInstrumentContext.getEffectiveStallSpeed() <= event.speed)
    {
        fsm.changeState(*togaLkThrottleState);
    }
}

void ManualThrottleState::onEvent(LeverChangeEvent& event)
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

void ManualThrottleState::onEvent(FdChangeEvent& event)
{
    // LOL DON'T CARE
}

void ManualThrottleState::onEvent(PowerModeChangeEvent& event)
{
    if (event.mode == EPowerMode::SELECTED || event.mode == EPowerMode::MANAGED)
    {
        fsm.changeState(*autoThrottleState);
    }
}

void ManualThrottleState::onEvent(EffectiveStallSpeedChangeEvent& event)
{
    if (flightInstrumentContext.getIndicatedAirspeed() <= event.speed)
    {
        fsm.changeState(*togaLkThrottleState);
    }
}

void ManualThrottleState::setTargetStateInstances(AutoThrottleState& autoThrottleState, TogaThrottleState& togaThrottleState,
    TogaLkThrottleState& togaLkThrottleState, IdleThrottleState& idleThrottleState)
{
    this->autoThrottleState = &autoThrottleState;
    this->togaThrottleState = &togaThrottleState;
    this->togaLkThrottleState = &togaLkThrottleState;
    this->idleThrottleState = &idleThrottleState;
}
