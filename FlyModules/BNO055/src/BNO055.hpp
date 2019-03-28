#ifndef __BNO055_HPP__
#define  __BNO055_HPP__

namespace bno055
{
constexpr uint8_t REG0_BNOCHIPID  = 0x00;
constexpr uint8_t REG0_ACCCHIPID  = 0x01;
constexpr uint8_t REG0_MAGCHIPID  = 0x02;
constexpr uint8_t REG0_GYRCHIPID  = 0x03;
constexpr uint8_t REG0_SWREVIDLO  = 0x04;
constexpr uint8_t REG0_SWREVIDHI  = 0x05;
constexpr uint8_t REG0_BLREVID    = 0x06;
constexpr uint8_t REG0_PAGEID     = 0x07;
constexpr uint8_t REG0_ACCDATAXLO = 0x08;
constexpr uint8_t REG0_ACCDATAXHI = 0x09;
constexpr uint8_t REG0_ACCDATAYLO = 0x0A;
constexpr uint8_t REG0_ACCDATAYHI = 0x0B;
constexpr uint8_t REG0_ACCDATAZLO = 0x0C;
constexpr uint8_t REG0_ACCDATAZHI = 0x0D;
constexpr uint8_t REG0_MAGDATAXLO = 0x0E;
constexpr uint8_t REG0_MAGDATAXHI = 0x0F;
constexpr uint8_t REG0_MAGDATAYLO = 0x10;
constexpr uint8_t REG0_MAGDATAYHI = 0x11;
constexpr uint8_t REG0_MAGDATAZLO = 0x12;
constexpr uint8_t REG0_MAGDATAZHI = 0x13;
constexpr uint8_t REG0_GYRDATAXLO = 0x14;
constexpr uint8_t REG0_GYRDATAXHI = 0x15;
constexpr uint8_t REG0_GYRDATAYLO = 0x16;
constexpr uint8_t REG0_GYRDATAYHI = 0x17;
constexpr uint8_t REG0_GYRDATAZLO = 0x18;
constexpr uint8_t REG0_GYRDATAZHI = 0x19;
constexpr uint8_t REG0_EULHDGLO   = 0x1A;
constexpr uint8_t REG0_EULHDGHI   = 0x1B;
constexpr uint8_t REG0_EULROLLGLO = 0x1C;
constexpr uint8_t REG0_EULROLLGHI = 0x1D;
constexpr uint8_t REG0_EULPUTCHLO = 0x1E;
constexpr uint8_t REG0_EULPUTCHHI = 0x1F;
constexpr uint8_t REG0_QUADATAWLO = 0x20;
constexpr uint8_t REG0_QUADATAWHI = 0x21;
constexpr uint8_t REG0_QUADATAXLO = 0x22;
constexpr uint8_t REG0_QUADATAXHI = 0x23;
constexpr uint8_t REG0_QUADATAYLO = 0x24;
constexpr uint8_t REG0_QUADATAYHI = 0x25;
constexpr uint8_t REG0_QUADATAZLO = 0x26;
constexpr uint8_t REG0_QUADATAZHI = 0x27;
constexpr uint8_t REG0_LIADATAXLO = 0x28;
constexpr uint8_t REG0_LIADATAXHI = 0x29;
constexpr uint8_t REG0_LIADATAYLO = 0x2A;
constexpr uint8_t REG0_LIADATAYHI = 0x2B;
constexpr uint8_t REG0_LIADATAZLO = 0x2C;
constexpr uint8_t REG0_LIADATAZHI = 0x2D;
constexpr uint8_t REG0_GRVDATAXLO = 0x2E;
constexpr uint8_t REG0_GRVDATAXHI = 0x2F;
constexpr uint8_t REG0_GRVDATAYLO = 0x30;
constexpr uint8_t REG0_GRVDATAYHI = 0x31;
constexpr uint8_t REG0_GRVDATAZLO = 0x32;
constexpr uint8_t REG0_GRVDATAZHI = 0x33;
constexpr uint8_t REG0_TEMP       = 0x34;

constexpr uint8_t REG0_CALIBSTAT     = 0x35;
constexpr uint8_t SYSCALIBSTATUSMASK = 0b11000000;
constexpr uint8_t GYRCALIBSTATUSMASK = 0b00110000;
constexpr uint8_t ACCCALIBSTATUSMASK = 0b00001100;
constexpr uint8_t MAGCALIBSTATUSMASK = 0b00000011;

constexpr uint8_t REG0_STRESULT = 0x36;
constexpr uint8_t STMCUMASK     = 0b00001000;
constexpr uint8_t STGYRMASK     = 0b00000100;
constexpr uint8_t STMAGMASK     = 0b00000010;
constexpr uint8_t STACCMASK     = 0b00000001;

constexpr uint8_t REG0_INTSTA   = 0x37;
constexpr uint8_t ACCNMMASK     = 0b10000000;
constexpr uint8_t ACCAMMASK     = 0b01000000;
constexpr uint8_t ACCHIGHGMASK  = 0b00100000;
constexpr uint8_t GYRHIGHRMASK  = 0b00001000;
constexpr uint8_t GYRAMMASK     = 0b00000100;

constexpr uint8_t REG0_SYSCLOCKSTATUS = 0x38;
constexpr uint8_t STMAINCLKMASK       = 0b00000001;

constexpr uint8_t REG0_SYSSTATUSCODE = 0x39;
constexpr uint8_t REG0_SYSERRORCODE  = 0x3A;

constexpr uint8_t REG0_UNITSEL          = 0x3B;
constexpr uint8_t ORIANDROIDWINDOWSMASK = 0b10000000;
constexpr uint8_t TEMPUNITMASK          = 0b00010000;
constexpr uint8_t EULUNITMASK           = 0b00000100;
constexpr uint8_t GYRUNITMASK           = 0b00000010;
constexpr uint8_t ACCUNITMASK           = 0b00000001;

constexpr uint8_t REG0_OPRMODE = 0x3C;
constexpr uint8_t OPERATIONMODEMASK = 0b00001111;

constexpr uint8_t REG0_PWRMODE = 0x3D;
constexpr uint8_t POWERMODEMASK = 0b00000011;


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