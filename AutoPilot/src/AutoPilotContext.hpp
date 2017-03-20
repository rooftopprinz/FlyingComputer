#ifndef AUTOPILOT_PILOTCONTEXT_HPP_
#define AUTOPILOT_PILOTCONTEXT_HPP_

struct AutoPilotContext
{
    virtual void eventLeverChange(double leverValue) = 0;
    virtual void eventAutoThrottleState(bool state) = 0;
    virtual void eventSpeedChange(double speed) = 0;
}

#endif