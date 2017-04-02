#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_PITCHPOWERMANAGER_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_PITCHPOWERMANAGER_HPP_

class PitchPowerManager
{
public:
    void start();
    void stop();
private:
    /** Update handlers **/ 
    void onLeverUpdate(ptree::Buffer& value);
    void onSpeedUpdate(ptree::Buffer& value);
    void onAutoPilotUpdate(ptree::Buffer& value);
};

#endif