#ifndef __ADXL345_HPP__
#define  __ADXL345_HPP__

namespace adxl345
{

constexpr uint8_t REGDEVID       = 0x00;
constexpr uint8_t REGTHRESHTAP   = 0x1D;
constexpr uint8_t REGOFFSETX     = 0x1E;
constexpr uint8_t REGOFFSETY     = 0x1F;
constexpr uint8_t REGOFFSETZ     = 0x20;
constexpr uint8_t REGDURATION    = 0x21;
constexpr uint8_t REGTAPLATENCY  = 0x22;
constexpr uint8_t REGTAPWINDOW   = 0x23;
constexpr uint8_t REGTHRESHACT   = 0x24;
constexpr uint8_t REGTHRESHINACT = 0x25;
constexpr uint8_t REGTIMEINACT   = 0x26;

constexpr uint8_t REGACTINACTCTL = 0x27;
constexpr uint8_t ACTACDCMASK    = 0b10000000;
constexpr uint8_t ACTXENMASK     = 0b01000000;
constexpr uint8_t ACTYENMASK     = 0b00100000;
constexpr uint8_t ACTZENMASK     = 0b00010000;
constexpr uint8_t INACTACDCMASK  = 0b00001000;
constexpr uint8_t INACTXENMASK   = 0b00000100;
constexpr uint8_t INACTYENMASK   = 0b00000010;
constexpr uint8_t INACTZENMASK   = 0b00000001;

constexpr uint8_t REGTHRESHFF    = 0x28;

constexpr uint8_t REGTIMEFF      = 0x29;

constexpr uint8_t REGTAPAXES     = 0x2A;
constexpr uint8_t SUPRESSMASK    = 0b00001000;
constexpr uint8_t TAPXENMASK     = 0b00000100;
constexpr uint8_t TAPYENMASK     = 0b00000010;
constexpr uint8_t TAPZENMASK     = 0b00000001;

constexpr uint8_t REGACTTAPSTAT  = 0x2B;
constexpr uint8_t ACTXSRCMASK    = 0b01000000;
constexpr uint8_t ACTYSRCMASK    = 0b00100000;
constexpr uint8_t ACTZSRCMASK    = 0b00010000;
constexpr uint8_t ASLEEPMASK     = 0b00001000;
constexpr uint8_t TAPXSRCMASK    = 0b00000100;
constexpr uint8_t TAPYSRCMASK    = 0b00000010;
constexpr uint8_t TAPZSRCMASK    = 0b00000001;

constexpr uint8_t REGBWRATE      = 0x2C;
constexpr uint8_t LOWPOWERMASK   = 0b00010000;
constexpr uint8_t RATEMASK       = 0b00001111;
enum class DataRate
{
    DR_0p10HZ,
    DR_0p20HZ,
    DR_0p39HZ,
    DR_0p78HZ,
    DR_1p56HZ,
    DR_3p13HZ,
    DR_6p26HZ,
    DR_12p5HZ,
    DR_25p0HZ,
    DR_50p0HZ,
    DR_100p0HZ,
    DR_200p0HZ,
    DR_400p0HZ,
    DR_800p0HZ,
    DR_1600p0HZ,
    DR_3200p0HZ
};

constexpr uint8_t REGPOWERCTL    = 0x2D;
constexpr uint8_t LINKMASK       = 0b00100000;
constexpr uint8_t AUTOSLEEPMASK  = 0b00010000;
constexpr uint8_t MEASUREMASK    = 0b00001000;
constexpr uint8_t SLEEPMASK      = 0b00000100;
constexpr uint8_t WAKEUPMASK     = 0b00000011;

constexpr uint8_t REGINTENABLE   = 0x2E;
constexpr uint8_t REGINTMAP      = 0x2F;
constexpr uint8_t REGINTSRC      = 0x30;
constexpr uint8_t DATARDYMASK    = 0b10000000;
constexpr uint8_t SINGLETAPMASK  = 0b01000000;
constexpr uint8_t DOUBLETAPMASK  = 0b00100000;
constexpr uint8_t ACTIVITYMASK   = 0b00010000;
constexpr uint8_t INACTIVITYMASK = 0b00001000;
constexpr uint8_t FREEFALLMASK   = 0b00000100;
constexpr uint8_t WATERMARKMASK  = 0b00000010;
constexpr uint8_t OVERRUNMASK    = 0b00000001;

constexpr uint8_t REGDATAFMT     = 0x31;
constexpr uint8_t SELFTESTMASK   = 0b10000000;
constexpr uint8_t SPIMASK        = 0b01000000;
constexpr uint8_t INTINVERTMASK  = 0b00100000;
constexpr uint8_t FULLRESMASK    = 0b00001000;
constexpr uint8_t JUSTIFYMASK    = 0b00000100;
constexpr uint8_t RANGEMASK      = 0b00000011;
enum class Range
{
    Range_2g,
    Range_4g,
    Range_8g,
    Range_16g
};

constexpr uint8_t REGDATAX0      = 0x32;
constexpr uint8_t REGDATAX1      = 0x33;
constexpr uint8_t REGDATAY0      = 0x34;
constexpr uint8_t REGDATAY1      = 0x35;
constexpr uint8_t REGDATAZ0      = 0x36;
constexpr uint8_t REGDATAZ1      = 0x37;

constexpr uint8_t REGFIFOCTL     = 0x38;
constexpr uint8_t FIFOMODEMASK   = 0b11000000;
constexpr uint8_t TRIGGERMASK    = 0b00110000;
constexpr uint8_t SAMPLESMASK    = 0b00001111;

enum class FifoMode
{
    Bypass,
    FIFO,
    Stream,
    Trigger
};

constexpr uint8_t REGFIFOSTAT    = 0x39;
constexpr uint8_t FIFOTRIGMASK   = 0b10000000;
constexpr uint8_t FIFOSZMASK     = 0b00111111;

class ADXL345
{
public:
    ADXL345(hwapi::II2C& pI2C)
        : mI2C(pI2C)
    {}
    void configure()
    {}
private:
    hwapi::II2C& mI2C;
};

} // adxl345

#endif // __ADXL345_HPP__