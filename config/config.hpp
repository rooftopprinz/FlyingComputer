struct grouping_t
{
    uint8_t id;
    std::vector<std::string> params;
};

struct flyItfConfig_t
{
    std::vector<std::string> mapping;
    std::vector<grouping_t> grouping;
};

struct config_t
{
    flyItfConfig_t flyItfConfig;
};
