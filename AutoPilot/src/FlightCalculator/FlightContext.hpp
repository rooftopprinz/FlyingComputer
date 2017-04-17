#ifndef AUTOPILOT_FLIGHTCALCULATOR_FLIGHTCONTEXT_HPP_
#define AUTOPILOT_FLIGHTCALCULATOR_FLIGHTCONTEXT_HPP_

class FlightContext : public IFlightContext
{
public:
    double getEffectiveStallSpeed() override;
    double getIndicatedAirspeed() override;
};

#endif