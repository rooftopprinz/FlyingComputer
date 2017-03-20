#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_TOGALKTHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_TOGALKTHROTTLESTATE_HPP_

#include <src/StateMachine/StateMachine.hpp>

class TogaLkThrottleState : public IState
{
public:
    void onEnter() override;
    void onExit() override;
    void onEvent(IEvent& event) override;
private:
}

#endif