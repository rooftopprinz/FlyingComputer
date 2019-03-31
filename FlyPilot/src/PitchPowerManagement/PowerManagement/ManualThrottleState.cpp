#include "ManualThrottleState.hpp"
#include <src/Contexts/IFlightInstrumentContext.hpp>

ManualThrottleState::ManualThrottleState(IFiniteStateMachine& fsm,
    IFlightInstrumentContext& flightInstrumentContext):
        CommonThrottleState(fsm, flightInstrumentContext)
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