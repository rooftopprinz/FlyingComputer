#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_MANUALTHROTTLESTATE_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_MANUALTHROTTLESTATE_HPP_

#include <src/StateMachine/StateMachine.hpp>

class ManualThrottleState : public IState
{
public:
    void onEnter() override;
    void onExit() override;
    void onEvent(IEvent& event) override;
private:
}

#endif