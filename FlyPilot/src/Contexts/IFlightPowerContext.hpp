#ifndef AUTOPILOT_CONTEXT_IFLIGHTPOWERCONTEXT_HPP_
#define AUTOPILOT_CONTEXT_IFLIGHTPOWERCONTEXT_HPP_

class IPitchPowerContext
{
public:
    virtual ~IPitchPowerContext() = default;
    virtual void setOutputPowerLeft(double power) = 0;
    virtual void setOutputPowerRight(double power) = 0;
    virtual void setOutputPower(double power) = 0;
private:
};

#endif