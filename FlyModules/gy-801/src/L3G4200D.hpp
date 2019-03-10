#ifndef __L3G4200D_HPP__
#define  __L3G4200D_HPP__

#include <exception>
#include <Logger.hpp>
#include <IHwApi.hpp>

namespace l3g4200d
{

// WHO_AM_I                     0x0F
constexpr uint8_t REGWHOAMI     = 0x0F;

// CTRL_REG1                    0x20
constexpr uint8_t REGCTLR1      = 0x20;
constexpr uint8_t DRMASK        = 0b11000000; // Data Rate Mask
constexpr uint8_t BWMASK        = 0b00110000; // Bandwidth Mask
constexpr uint8_t POWERDOWNMASK = 0b00001000; // Power Down Mask
constexpr uint8_t ZENMASK       = 0b00000100; // Z Axis Enable Mask
constexpr uint8_t YENMASK       = 0b00000010; // Y Axis Enable Mask
constexpr uint8_t XENMASK       = 0b00000001; // X Axis Enable Mask

enum class DataRate
{
    DR_100HZ,
    DR_200HZ,
    DR_400HZ,
    DR_800HZ
};

enum class Bandwidth
{
    BW_LOW,
    BW_LOW_MID,
    BW_HIGH_MID,
    BW_HIGH
};

// CTRL_REG2                    0x21
constexpr uint8_t REGCTLR2      = 0x21;
constexpr uint8_t HPMMASK       = 0b00110000; // High Pass Filter Mode Mask
constexpr uint8_t HPCMASK       = 0b00001111; // High Pass Filter Cutoff Mask

enum  class HighPassFilterMode
{
    NormalReset,
    ReferenceSignal,
    Normal,
    AutoReset
};

// CTRL_REG3                    0x22
constexpr uint8_t REGCTLR3      = 0x22;
constexpr uint8_t ENMASK        = 0b10000000; // Enabled Mask
constexpr uint8_t BSAENMASK     = 0b01000000; // Boot Status Enabled Mask
constexpr uint8_t ACTVMASK      = 0b00100000; // Active Mask
constexpr uint8_t PPODMASK      = 0b00010000; // Push-Pull or Open Drain Mask
constexpr uint8_t DRDYENMASK    = 0b00001000; // Data Ready Mask
constexpr uint8_t WTMENMASK     = 0b00000100; // Watermark Interrupt Enabled Mask
constexpr uint8_t OVRUNENMASK   = 0b00000010; // Overun Interrupt Enabled Mask 
constexpr uint8_t EMPTYENMASK   = 0b00000001; // Empty Interrupt Enabled Mask

// CTRL_REG4                    0x23
constexpr uint8_t REGCTLR4      = 0x23;
constexpr uint8_t BDUMASK       = 0b10000000; // Block Data Update Mask
constexpr uint8_t BLEMASK       = 0b01000000; // Big Little Endian Mask
constexpr uint8_t FSSELMASK     = 0b00110000; // Full Scale Selection Mask
constexpr uint8_t STESTMASK     = 0b00000110; // Self Test Mask
constexpr uint8_t SPIMODMASK    = 0b00000110; // SPI Mode Mask

enum  class FullScaleSelect
{
    FS_250dps,
    FS_500dps,
    FS_2000dps,
    FS_2000dps_2
};

enum  class SeltTestMode
{
    Disabled,
    Test0,
    RSV0,
    Test1
};

// CTRL_REG5                    0x24
constexpr uint8_t REGCTLR5    = 0x24;
constexpr uint8_t BOOTMASK    = 0b10000000; // Reboot Memory Content Mask
constexpr uint8_t FIFOENMASK  = 0b01000000; // Fifo Enabled Mask
constexpr uint8_t HPFENMASK   = 0b00010000; // High Pass Filter Enabled Mask
constexpr uint8_t HSELMASK    = 0b00001100; // Interrupt Selection Mask
constexpr uint8_t OUTSELMASK  = 0b00000011; // Output Selection Mask

enum class InterruptSelection
{
    LPF1,
    LPF1_HPF,
    LPF1_HPEN_LPF2,
    LPF1_HPEN_LPF2_X
};

enum class OutputSelection
{
    LPF1,
    LPF1_HPF,
    LPF1_HPEN_LPF2,
    LPF1_HPEN_LPF2_X
};

// EFERENCE/DATACAPTURE         0x26
constexpr uint8_t REGREF      = 0x25;
// OUT_TEMP                     0x26
constexpr uint8_t REGTEMP     = 0x26;

// STATUS_REG                   0x25       
constexpr uint8_t REGSTAT     = 0x27;
constexpr uint8_t ZYXORMASK   = 0b10000000; // ZYX Overrun Mask
constexpr uint8_t ZORMASK     = 0b01000000; // Z Overrun Mask
constexpr uint8_t YORMASK     = 0b00100000; // Y Overrun Mask
constexpr uint8_t XORMASK     = 0b00010000; // X Overrun Mask
constexpr uint8_t ZYXDAMASK   = 0b00001000; // ZYX Data Available Mask
constexpr uint8_t ZDAMASK     = 0b00000100; // Z Overrun Mask
constexpr uint8_t YDAMASK     = 0b00000010; // Y Overrun Mask
constexpr uint8_t XDAMASK     = 0b00000001; // X Overrun Mask

// OUT_X_L                      0x28
constexpr uint8_t REGOXL      = 0x28;
// OUT_X_H                      0x29
constexpr uint8_t REGOXH      = 0x29;
// OUT_Y_L                      0x2A
constexpr uint8_t REGOYL      = 0x2A;
// OUT_Y_H                      0x2B
constexpr uint8_t REGOYH      = 0x2B;
// OUT_Z_L                      0x2C
constexpr uint8_t REGOZL      = 0x2C;
// OUT_Z_LH                     0x2D
constexpr uint8_t REGOZH      = 0x2D;

// FIFO_CTRL_REG                0x2E
constexpr uint8_t REGFIFOCTRL  = 0x2E;
constexpr uint8_t FIFOMODEMASK = 0b11100000; // Fifo Mode Mask
constexpr uint8_t WATERMVMASK  = 0b00011111; // Watermark Value Mask

enum class FifoMode
{
    Bypass,
    Fifo,
    Stream,
    StreamFifo,
    BypassStream
};

// FIFO_SRC_REG                 0x2F
constexpr uint8_t REGFIFOSRC  = 0x2F;
constexpr uint8_t WATERMSMASK = 0b10000000; // Watermark Status Mask
constexpr uint8_t OVERRUNMASK = 0b01000000; // Overrun Status Mask
constexpr uint8_t EMPTYMASK   = 0b00100000; // Empty Status Mask
constexpr uint8_t FIFOCOUNT   = 0b00011111; // FIFO Count Mask

// INT1_CFG                      0x30
constexpr uint8_t REGINT1CFG  = 0x30;
constexpr uint8_t ANDORMASK   = 0b10000000; // And/Or Interrupt Events Mask
constexpr uint8_t LIRMASK     = 0b01000000; // Latched Interrupt Mask
constexpr uint8_t ZHIE        = 0b00100000; // Enable Interrupt Generation Mask
constexpr uint8_t ZLIE        = 0b00010000; // Enable Interrupt Generation Mask
constexpr uint8_t YHIE        = 0b00001000; // Enable Interrupt Generation Mask
constexpr uint8_t YLIE        = 0b00000100; // Enable Interrupt Generation Mask
constexpr uint8_t XHIE        = 0b00000010; // Enable Interrupt Generation Mask
constexpr uint8_t XLIE        = 0b00000001; // Enable Interrupt Generation Mask

constexpr uint8_t REGINT1SRC  = 0x31;
constexpr uint8_t REGINT1TXH  = 0x32;
constexpr uint8_t REGINT1TXL  = 0x33;
constexpr uint8_t REGINT1TYH  = 0x34;
constexpr uint8_t REGINT1TYL  = 0x35;
constexpr uint8_t REGINT1TZH  = 0x36;
constexpr uint8_t REGINT1TZL  = 0x37;
constexpr uint8_t REGINT1DUR  = 0x38;

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

class L3G4200D
{
public:
    L3G4200D(hwapi::II2C& pI2C)
        : mI2C(pI2C)
    {}

    void configure(DataRate pDataRate, Bandwidth pBandwidth)
    {
        if (0b11010011 != getRegister(REGWHOAMI))
        {
            throw std::runtime_error(std::string("L3G4200D is not recognized!"));
        }

        uint8_t ctrl = setMasked(DRMASK, (unsigned)pDataRate) | setMasked(BWMASK, (unsigned)pBandwidth)|POWERDOWNMASK|ZENMASK|YENMASK|XENMASK;
        setRegister(REGCTLR1, ctrl);
        ctrl = setMasked(FSSELMASK, (unsigned)FullScaleSelect::FS_2000dps)|BDUMASK;
        setRegister(REGCTLR4, ctrl);
        ctrl = setMasked(FIFOENMASK, true)|setMasked(OUTSELMASK, (unsigned)OutputSelection::LPF1);
        setRegister(REGCTLR5, ctrl);
    }

    void bypass()
    {
        uint8_t ctrl = setMasked(FIFOMODEMASK, (unsigned)FifoMode::Bypass);
        setRegister(REGFIFOCTRL, ctrl);
    }

    void fifo()
    {
        uint8_t ctrl = setMasked(FIFOMODEMASK, (unsigned)FifoMode::Fifo);
        setRegister(REGFIFOCTRL, ctrl);
    }

    size_t read(uint8_t* pDataXYZ)
    {
        Logless("L3G4200D::read READ FIFO");
        uint8_t src = getRegister(REGFIFOSRC);
        uint8_t sz = getUnmasked(FIFOCOUNT, src)+1;
        const size_t rc = (sz/5) + !!(sz%5);

        for (size_t i=0; i<rc; i++)
        {
            size_t rsz = 30;
            if (i==(rc-1) && !!(sz%5))
            {
                rsz = (sz%5)*6;
            }
            getRegister(REGOXL|0x80, pDataXYZ+30*i, rsz);
        }
        getRegister(REGFIFOSRC);
        bypass();
        fifo();
        return sz;
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

}

#endif // __L3G4200D_HPP__
