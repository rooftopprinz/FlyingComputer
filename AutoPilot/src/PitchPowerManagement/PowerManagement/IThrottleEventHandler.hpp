#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_ITHROTTLEEVENTHANDLER_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_ITHROTTLEEVENTHANDLER_HPP_

#include "PowerEvents.hpp"

class IThrottleEventHandler
{
public:
    virtual ~IThrottleEventHandler() = default;
    virtual void onEvent(SpeedChangeEvent& event) = 0;
    virtual void onEvent(LeverChangeEvent& event) = 0;
    virtual void onEvent(FdChangeEvent& event) = 0;
    virtual void onEvent(PowerModeChangeEvent& event) = 0;
    virtual void onEvent(EffectiveStallSpeedChangeEvent& event) = 0;
};

#endif