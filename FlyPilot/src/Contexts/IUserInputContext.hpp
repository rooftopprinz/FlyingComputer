#ifndef AUTOPILOT_CONTEXT_IUSERINPUTCONTEXT_HPP_
#define AUTOPILOT_CONTEXT_IUSERINPUTCONTEXT_HPP_

class IUserInputContext
{
public:
    virtual ~IUserInputContext() = default;
    virtual double getInputElevator() = 0;
    virtual double getInputPowerLeft() = 0;
    virtual double getInputPowerRight() = 0;
    virtual double getAltSel() = 0;
    virtual double getSpdSel() = 0;
    virtual double getVsSel() = 0;
};

#endif