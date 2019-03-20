#ifndef __BNO055_HPP__
#define  __BNO055_HPP__

namespace bno055
{

inline uint64_t getUnmasked(uint64_t mask, uint64_t value)
{
    for (; !(mask&1); mask = (mask>>1), value = (value>>1));
    return mask&value;
}

inline uint64_t setMasked(uint64_t mask, uint64_t value)
{
    auto omask = mask;
    for (; !(mask&1); mask = (mask>>1), value = (value<<1));
    return value&omask;
}

class BNO055
{
public:
    BNO055(hwapi::II2C& pI2C)
        : mI2C(pI2C)
    {}

    void configure()
    {
        if (0b11100101 != getRegister(REGDEVID))
        {
            throw std::runtime_error(std::string("BNO055 is not recognized!"));
        }
    }
private:
    void setRegister(uint8_t pRegister, uint8_t pValue)
    {
        mI2C.writeBlock(pRegister, &pValue, 1);
    }

    uint8_t getRegister(uint8_t pRegister)
    {
        uint8_t rv;
        mI2C.readBlock(pRegister, &rv, 1);
        return rv;
    }

    void getRegister(uint8_t pRegister, uint8_t* pData, size_t pCount)
    {
        mI2C.readBlock(pRegister, pData, pCount);
    }

    hwapi::II2C& mI2C;
};

} // bno055

#endif // __BNO055_HPP__