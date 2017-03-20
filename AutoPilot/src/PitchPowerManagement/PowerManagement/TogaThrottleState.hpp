#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_TOGATHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_TOGATHROTTLESTATE_HPP_

#include <src/StateMachine/StateMachine.hpp>

class TogaThrottleState : public IState
{
public:
    void onEnter() override;
    void onExit() override;
    void onEvent(IEvent& event) override;
private:
}

#endif