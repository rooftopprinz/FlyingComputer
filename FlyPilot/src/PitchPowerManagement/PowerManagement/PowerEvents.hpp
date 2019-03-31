#ifndef AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_POWEREVENTS_HPP_
#define AUTOPILOT_PITCHPOWERMANAGEMENT_POWERMANAGEMENT_POWEREVENTS_HPP_

enum class EApOperatingMode {MANUAL, SELECTED, MANAGED};
using EPowerMode = EApOperatingMode;

struct SpeedChangeEvent
{
    double speed;
};

struct LeverChangeEvent
{
    double lever;
};

struct FdChangeEvent
{
    bool isEnabled;
};

struct PowerModeChangeEvent
{
    EPowerMode mode;
};

struct EffectiveStallSpeedChangeEvent
{
    double speed;
};

#endif