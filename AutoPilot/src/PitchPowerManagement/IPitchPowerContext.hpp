#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_IPITCHPOWERMANAGER_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_IPITCHPOWERMANAGER_HPP_

class IPitchPowerContext
{
public:
    virtual ~IPitchPowerContext() = default;
    virtual void setOutputPowerLeft(double power) = 0;
    virtual void setOutputPowerRight(double power) = 0;
    virtual void setOutputElevator(double angle) = 0;
    virtual double getInputElevator() = 0;
    virtual double getInputPowerLeft() = 0;
    virtual double getInputPowerRight() = 0;
    virtual double getAltSel() = 0;
    virtual double getSpdSel() = 0;
    virtual double getVsSel() = 0;

private:
};

#endif