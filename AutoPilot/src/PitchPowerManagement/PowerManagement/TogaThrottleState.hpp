#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_TOGATHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_TOGATHROTTLESTATE_HPP_

#include <src/StateMachine/StateMachine.hpp>
#include <src/PitchPowerManagement/IPitchPowerContext.hpp>
#include "ThrottleEventHandler.hpp"
#include "AutoThrottleState.hpp"
#include "TogaThrottleState.hpp"
#include "TogaLkThrottleState.hpp"
#include "TogaLkThrottleState.hpp"

class TogaThrottleState : public IState, public ThrottleEventHandler
{
public:
    void onEnter() override;
    void onExit() override;
    void onEvent(SpeedChangeEvent& event) override;
    void onEvent(LeverChangeEvent& event) override;
    void onEvent(FdChangeEvent& event) override;
    void onEvent(PowerModeChangeEvent& event) override;
    void onEvent(VrsChangeEvent& event) override;
private:
};

#endif