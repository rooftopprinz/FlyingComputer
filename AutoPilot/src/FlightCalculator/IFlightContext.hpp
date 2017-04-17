#ifndef AUTOPILOT_FLIGHTCALCULATOR_IFLIGHTCONTEXT_HPP_
#define AUTOPILOT_FLIGHTCALCULATOR_IFLIGHTCONTEXT_HPP_

class IFlightContext
{
public:
    virtual double getEffectiveStallSpeed() = 0;
    virtual double getIndicatedAirspeed() = 0;
};

#endif