#ifndef AUTOPILOT_TEST_MOCK_IFLIGHTCONTEXTMOCK_HPP_
#define AUTOPILOT_TEST_MOCK_IFLIGHTCONTEXTMOCK_HPP_

#include <gmock/gmock.h>

#include <src/FlightCalculator/IFlightContext.hpp>

struct FlightContextMock : public IFlightContext
{
    MOCK_METHOD0(getEffectiveStallSpeed, double());
    MOCK_METHOD0(getIndicatedAirspeed, double());
};

#endif