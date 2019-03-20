#include <IHwApi.hpp>
#include <exception>
#include <pigpiod_if2.h>
#include <Logger.hpp>
#include <iomanip>

namespace hwapi
{

std::string toHexString(const uint8_t* pData, size_t size)
{
    std::stringstream ss;;
    for (size_t i=0; i<size; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << unsigned(pData[i]);
    }

    return ss.str();
}

static int gGpioHandle = -1;

class I2C : public II2C
{
public:
    I2C(uint8_t pAddress)
        : mAddress(pAddress)
    {

        Logless("DBG I2C::I2C opening address=_ for gpio=_", unsigned(pAddress), gGpioHandle);
        mHandle = i2c_open(gGpioHandle, 1, pAddress, 0);
        if (mHandle<0)
        {
            throw std::runtime_error(std::string{} + "i2c failed! handle=" + std::to_string(mHandle));
        }
    }

    ~I2C()
    {
        if (mHandle>=0)
        {
            i2c_close(gGpioHandle, mHandle);
        }
    }
    int writeBlock(unsigned pReg, uint8_t* pBuf, unsigned pCount)
    {
        int rv = i2c_write_i2c_block_data(gGpioHandle, mHandle, pReg, (char*)pBuf, pCount);
        Logless("DBG I2C[_]::writBlock rv:_ [_]=_", mAddress, rv, pReg, BufferLog(pCount, pBuf));
        return rv;
    }
    int readBlock(unsigned pReg, uint8_t* pBuf, unsigned pCount)
    {
        int rv = i2c_read_i2c_block_data(gGpioHandle, mHandle, pReg, (char*)pBuf, pCount);
        Logless("DBG I2C[_]::readBlock rv:_ [_]=_", mAddress, rv, pReg, BufferLog(pCount, pBuf));
        return rv;
    }
private:
    int mHandle;
    int mAddress;
};

std::shared_ptr<II2C>  getI2C(uint8_t pAddress)
{
    return std::make_shared<I2C>(pAddress);
}

void setup()
{
    char port[] = {'8','8','8','8',0};
    gGpioHandle = pigpio_start(nullptr, port);
    if (gGpioHandle<0)
    {
        throw std::runtime_error(std::string{} + "connecting to pigpiod failed! handle=" + std::to_string(gGpioHandle));
    }
    Logless("DBG hwapi::setup gpiohandle = _", gGpioHandle);
}

void teardown()
{
    if (gGpioHandle>=0)
    {
        pigpio_stop(gGpioHandle);
    }
}

}