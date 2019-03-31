#include <src/Contexts/IFlightInstrumentContext.hpp>
#include <src/Contexts/IFlightPowerContext.hpp>
#include <src/Contexts/IUserInputContext.hpp>

#include "ManualThrottleState.hpp"
#include "AutoThrottleState.hpp"
#include "TogaLkThrottleState.hpp"

#include "CommonThrottleState.hpp"

CommonThrottleState::CommonThrottleState(IFiniteStateMachine& fsm,
    IFlightInstrumentContext& flightInstrumentContext):
    fsm(fsm),
    flightInstrumentContext(flightInstrumentContext)
{

}
CommonThrottleState::~CommonThrottleState()
{

}
void CommonThrottleState::onEvent(SpeedChangeEvent& event)
{
    if (flightInstrumentContext.getEffectiveStallSpeed() >= event.speed)
    {
        fsm.changeState(*togaLkThrottleState);
    }
}

void CommonThrottleState::onEvent(LeverChangeEvent& event)
{
    // TODO: adjust power setting directly on lever
}

void CommonThrottleState::onEvent(FdChangeEvent& event)
{
    // N/A
}

void CommonThrottleState::onEvent(PowerModeChangeEvent& event)
{
    if (event.mode == EPowerMode::MANUAL)
    {
        fsm.changeState(*manualThrottleState);
    }
    else if (event.mode == EPowerMode::SELECTED || event.mode == EPowerMode::MANAGED)
    {
        fsm.changeState(*autoThrottleState);
    }
}

void CommonThrottleState::onEvent(EffectiveStallSpeedChangeEvent& event)
{
    if (flightInstrumentContext.getIndicatedAirspeed() <= event.speed)
    {
        fsm.changeState(*togaLkThrottleState);
    }
}

void CommonThrottleState::setTargetStateInstances(ManualThrottleState& manualThrottleState, AutoThrottleState& autoThrottleState,
    TogaLkThrottleState& togaLkThrottleState)
{
    this->manualThrottleState = &manualThrottleState;
    this->autoThrottleState = &autoThrottleState;
    this->togaLkThrottleState = &togaLkThrottleState;
}
