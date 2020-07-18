#ifndef __FLYITF_HPP__
#define __FLYITF_HPP__

#include <logless/Logger.hpp>

#include <propertytree/Client.hpp>
#include <propertytree/Property.hpp>

#include <ConfigTree/ConfigTree.hpp>
#include <ConfigTree/ConfigTreeParser.hpp>
#include <ConfigTree/ConfigLoader.hpp>
#include <ConfigTree/ConfigPrinter.hpp>

#include <config/config_defs.hpp>


class FlyItf
{
public:
    FlyItf()
        : mTree(propertytree::ClientConfig{"127.0.0.1", 12345})
    {
        std::ifstream configFile("config.xml");
        std::string configXml((std::istreambuf_iterator<char>(configFile)),
                        std::istreambuf_iterator<char>());

        ConfigParser parser(configXml);
        LoadConfig("config", &parser.root, mConfig);

        auto configStr = GetPrintedString(mConfig);
        Logless("DBG FlyItf: Parsed Config: _", configStr.c_str());

        auto root = mTree.root();
        auto flyItf = root.createOrGet("FLYITF");

        mPropInputYokeRoll  = flyItf.createOrGet("YOKE_ROLL");
        mPropInputYokePitch = flyItf.createOrGet("YOKE_PITCH");
        mPropInputRudder = flyItf.createOrGet("RUDDER");
        mPropInputThrottleLeft = flyItf.createOrGet("THROTTLE_LEFT");
        mPropInputThrottleRight = flyItf.createOrGet("THROTTLE_RIGHT");
        mPropInputSpeedBrake = flyItf.createOrGet("SPEED_BRAKE");
        mPropInputPitchTrim = flyItf.createOrGet("PITCH_TRIM");
        mPropInputFlap = flyItf.createOrGet("FLAP");
        mPropInputGear = flyItf.createOrGet("GEAR");
    }

    void run()
    {
        while (true);
    }

private:
    config_t mConfig;
    propertytree::Client mTree;

    bool mSystemReady = false;
    std::mutex mSystemReadyMutex;
    std::condition_variable mSystemReadyCv;

    propertytree::Property mPropInputYokeRoll;
    propertytree::Property mPropInputYokePitch;
    propertytree::Property mPropInputRudder;
    propertytree::Property mPropInputThrottleLeft;
    propertytree::Property mPropInputThrottleRight;
    propertytree::Property mPropInputSpeedBrake;
    propertytree::Property mPropInputPitchTrim;
    propertytree::Property mPropInputFlap;
    propertytree::Property mPropInputGear;
};

#endif // __FLYITF_HPP__