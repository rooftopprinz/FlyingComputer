#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_PITCHPOWERMANAGER_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_PITCHPOWERMANAGER_HPP_

#include "IPitchPowerContext.hpp"

class PitchPowerContext : private IPitchPowerContext
{
public:
    void setOutputPowerLeft(double power) override;
    void setOutputPowerRight(double power) override;
    void setOutputPower(double power) override;
    void setOutputElevator(double angle) override;
    double getInputElevator() override;
    double getInputPowerLeft() override;
    double getInputPowerRight() override;
    double getAltSel() override;
    double getSpdSel() override;
    double getVsSel() override;

private:
};

#endif