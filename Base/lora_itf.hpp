#include <functional>

struct ISpi
{
    virtual int read(char *data, unsigned count) = 0;
    virtual int write(char *data, unsigned count) = 0;
    virtual int xfer(char *dataOut, char *dataIn, unsigned count) = 0;
};

struct IGpio
{
    virtual int setMode(unsigned gpio, unsigned mode) = 0;
    virtual int get(unsigned gpio) = 0;
    virtual int set(unsigned gpio, unsigned level) = 0;
    virtual int callback(unsigned userGpio, unsigned edge, std::function<void()> cb) = 0;
};