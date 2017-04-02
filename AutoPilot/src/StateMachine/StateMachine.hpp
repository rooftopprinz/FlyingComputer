#ifndef AUTOPILOT_STATEMACHINE_STATEMACHINE_HPP_
#define AUTOPILOT_STATEMACHINE_STATEMACHINE_HPP_

struct IState
{
    virtual ~IState() = default;
    virtual void onEnter() = 0;
    virtual void onExit() = 0;
};

struct IFiniteStateMachine
{
    virtual ~IFiniteStateMachine() = default;
    virtual void changeState(IState& target) = 0;
};

#endif