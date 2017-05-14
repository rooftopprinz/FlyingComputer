#ifndef AUTOPILOT_TEST_MOCK_IFLIGHTINSTRUMENTMOCK_HPP_
#define AUTOPILOT_TEST_MOCK_IFLIGHTINSTRUMENTMOCK_HPP_

#include <gmock/gmock.h>

#include <src/Contexts/IFlightInstrumentContext.hpp>

struct FlightInstrumentContextMock : public IFlightInstrumentContext
{
    MOCK_METHOD0(getEffectiveStallSpeed, double());
    MOCK_METHOD0(getIndicatedAirspeed, double());
};

#endif