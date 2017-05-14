#ifndef AUTOPILOT_CONTEXT_IFLIGHTINSTRUMENTCONTEXT_HPP_
#define AUTOPILOT_CONTEXT_IFLIGHTINSTRUMENTCONTEXT_HPP_

class IFlightInstrumentContext
{
public:
    virtual ~IFlightInstrumentContext() = default;
    virtual double getEffectiveStallSpeed() =0;
    virtual double getIndicatedAirspeed() =0;
};

#endif