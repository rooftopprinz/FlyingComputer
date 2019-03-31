#ifndef AUTOPILOT_TEST_MOCK_IFINITESTATEMACHINEMOCK_HPP_
#define AUTOPILOT_TEST_MOCK_IFINITESTATEMACHINEMOCK_HPP_

#include <gmock/gmock.h>

#include <src/StateMachine/StateMachine.hpp>

struct FiniteStateMachineMock : public IFiniteStateMachine
{
    MOCK_METHOD1(changeState, void(IState&));
};

#endif