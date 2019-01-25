// RegFifo      0x00
constexpr uint8_t RegFifo                   = 0x0;

// RegOpMode    0x01
constexpr uint8_t RegOpMode                 = 0x01;
constexpr uint8_t LongRangeModeMask         = 0b10000000;
constexpr uint8_t AccessSharedRegMask       = 0b01000000; // AccessSharedReg
constexpr uint8_t LowFrequencyModeOnMask    = 0b00001000; // LowFrequencyModeOn
constexpr uint8_t ModeMask                  = 0b00000111; // Mode

enum AccessSharedReg            // This bit operates when device is in Lora mode; if set it allows access to FSK registers page located in address space (0x0D:0x3F) while in LoRa mode.
{
    LORA,                       // ccess LoRa registers page 0x0D:0x3F
    FSK                         // Access FSK registers page (in mode LoRa) 0x0D:0x3F
};

enum Mode                       // Device modes
{
    SLEEP,                      // Sleep
    STDBY,                      // Standby
    FSTX,                       // Frequency synthesis TX
    TX,                         // TX
    FSRX,                       // Frequency synthesis RX
    RXCONTINUOUS,               // Receive continuous
    RXSINGLE,                   // Receive single
    CAD                         // Channel activity detection
};

// RegFrMsb     0x06
constexpr uint8_t RegFrMsb                  = 0x06; // MSB (23:16) of RF carrier frequency
// RegFrMid     0x07
constexpr uint8_t RegFrMid                  = 0x07; // MSB (15:8) of RF carrier frequency
// RegFrLsb     0x08
constexpr uint8_t RegFrLsb                  = 0x08; // MSB (7:0) of RF carrier frequency

inline uint64_t convertRegFrToFrequency(uint32_t regfr, uint64_t fxosc) {return fxosc*regfr/524288;}

// RegPaConfig  0x09
constexpr uint8_t RegPaConfig               = 0x09;
constexpr uint8_t PaSelectMask              = 0b10000000; // PaSelect
constexpr uint8_t MaxPowerMask              = 0b01110000; // MaxPower
constexpr uint8_t OutputPowerMask           = 0b00001111; // OutputPower

enum PaSelect                   // Selects PA output pin
{
    RFO,                        // RFO pin. Output power is limited to +14 dBm.
    PA_BOOST                    // PA_BOOST pin. Output power is limited to +20 dBm.
};

inline double converMaxPowerToPmax(uint8_t maxPower) {return 10.8+0.6*maxPower;}

inline double convertOutputPowerToPout(bool isBoosted, double pmax, uint8_t outputPower)
{
    if (isBoosted)
        return pmax - 15 + outputPower;
    return 2 + outputPower;
}

// RegPaRamp    0x0A
constexpr uint8_t RegPaRamp                 = 0x0A;
constexpr uint8_t PaRampMask                = 0b00001111; // PaRamp

enum PaRamp                     // Rise/Fall time of ramp up/down in FS
{
    RAMP_3P4_MS,                   // 3.4 ms
    RAMP_2_MS,                     // 2 ms
    RAMP_1_MS,                     // 1 ms
    RAMP_500_US,                   // 500 us
    RAMP_250_US,                   // 250 us
    RAMP_125_US,                   // 125 us
    RAMP_100_US,                   // 100 us
    RAMP_62_US,                    // 62 us
    RAMP_50_US,                    // 50 us
    RAMP_40_US,                    // 40 us
    RAMP_31_US,                    // 31 us
    RAMP_25_US,                    // 25 us
    RAMP_20_US,                    // 20 us
    RAMP_15_US,                    // 15 us
    RAMP_12_US,                    // 12 us
    RAMP_10_US,                    // 10 us
};

// RegPaRamp    0x0B
constexpr uint8_t RegOcp                 = 0x0B;
constexpr uint8_t OcpOnMask                 = 0b00100000; // OcpOn
constexpr uint8_t OcpTrimMask               = 0b00011111; // OcpTrim

inline double convertOcpTrimToImax(uint8_t ocpTrim)
{
    if (ocpTrim <= 15)
        return 45+5*ocpTrim;
    return -30+10*ocpTrim;
}

// RegLna       0x0C
constexpr uint8_t RegLna                    = 0x0C;
constexpr uint8_t LnaGainMask               = 0b11100000; // LnaGain
constexpr uint8_t LnaBoostLfMask            = 0b00011000; // LnaBoostLf
constexpr uint8_t LnaBoostHfMask            = 0b00000011; // LnaBoostHf

enum LnaGain                    // LNA Gain Setting
{
    G1 = 1,                     // G1 = maximum gain
    G2,                         // G2
    G3,                         // G3
    G4,                         // G4
    G5,                         // G5
    G6                          // G6 = minimum gain
};

enum LnaBoostLf                 // Low Frequency (RFI_LF) LNA current adjustment
{
    DEFAULT                     // Default LNA current
};

enum LnaBoostHf                 // High Frequency (RFI_HF) LNA current adjustment
{
    DEFAULT,                    // Default LNA current
    BOOST_ON                    // Boost on, 150% LNA current
};


// RegFifoAddrPtr               0x0D
constexpr uint8_t RegFifoAddrPtr            = 0x0D; // SPI interface address pointer in FIFO data buffer
// RegFifoTxBaseAdd             0x0E
constexpr uint8_t RegFifoTxBaseAdd          = 0x0E; // Write base address in FIFO data buffer for TX modulator
// RegFifoRxBaseAdd             0x0F
constexpr uint8_t RegFifoRxBaseAdd          = 0x0F; // Read base address in FIFO data buffer for RX demodulator
// RegFifoRxCurrentAddr         0x10
constexpr uint8_t RegFifoRxCurrentAddr      = 0x10; // Start address (in data buffer) of last packet received

// RegIrqFlagsMaskMask          0x11
constexpr uint8_t RegIrqFlagsMaskMask       = 0x11;
constexpr uint8_t RxTimeoutMaskMask         = 0b10000000; // Timeout interrupt mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t RxDoneMaskMask            = 0b01000000; // Packet reception complete interrupt mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t PayloadCrcErrorMaskMask   = 0b00100000; // Payload CRC error interrupt mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t ValidHeaderMaskMask       = 0b00010000; // Valid header received in Rx mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t TxDoneMaskMask            = 0b00001000; // FIFO Payload transmission complete interrupt mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t CadDoneMaskMask           = 0b00000100; // CAD complete interrupt mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t FhssChangeChannelMaskMask = 0b00000010; // FHSS change channel interrupt mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t CadDetectedMaskMask       = 0b00000001; // Cad Detected Interrupt Mask: setting this bit masks the corresponding IRQ in RegIrqFlags

// RegIrqFlags                  0x12
constexpr uint8_t RegIrqFlags               = 0x12;
constexpr uint8_t RxTimeoutMask             = 0b10000000; // Timeout interrupt: writing a 1 clears the IRQ
constexpr uint8_t RxDoneMask                = 0b01000000; // Packet reception complete interrupt: writing a 1 clears the IRQ
constexpr uint8_t PayloadCrcErrorMask       = 0b00100000; // Payload CRC error interrupt: writing a 1 clears the IRQ
constexpr uint8_t ValidHeaderMask           = 0b00010000; // Valid header received in Rx mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t TxDoneMask                = 0b00001000; // FIFO Payload transmission complete interrupt: writing a 1 clears the IRQ
constexpr uint8_t CadDoneMask               = 0b00000100; // CAD complete: write to clear: writing a 1 clears the IRQ
constexpr uint8_t FhssChangeChannelMask     = 0b00000010; // FHSS change channel interrupt: writing a 1 clears the IRQ
constexpr uint8_t CadDetectedMask           = 0b00000001; // Valid Lora signal detected during CAD operation: writing a 1 clears the IRQ

// RegRxNbBytes                 0x13
constexpr uint8_t RegRxNbBytes              = 0x13; // Number of payload bytes of latest packet received

// RegRxHeaderCntValueMsb       0x14
constexpr uint8_t RegRxHeaderCntValueMsb    = 0x14; // Number of valid headers received since last transition into Rx mode, MSB(15:8). Header and packet counters are reseted in Sleep mode.
// RegRxHeaderCntValueLsb       0x15
constexpr uint8_t RegRxHeaderCntValueLsb    = 0x15; // Number of valid headers received since last transition into Rx mode, LSB(7:0). Header and packet counters are reseted in Sleep mode.

// RegRxPacketCntValueMsb       0x16
constexpr uint8_t RegRxPacketCntValueMsb    = 0x16; // Number of valid packets received since last transition into Rx mode, MSB(15:8). Header and packet counters are reseted in Sleep mode.
// RegRxPacketCntValueLsb       0x17
constexpr uint8_t RegRxPacketCntValueLsb    = 0x17; // Number of valid packets received since last transition into Rx mode, LSB(7:0). Header and packet counters are reseted in Sleep mode.

// RegModemStat                 0x18
constexpr uint8_t RegModemStat              = 0x18;
constexpr uint8_t RxCodingRateMask          = 0b11100000; // Coding rate of last header received
constexpr uint8_t ModemClearMask            = 0b00010000; // Modem Clear
constexpr uint8_t HeaderInfoValidMask       = 0b00001000; // Header Info Valid
constexpr uint8_t RxOnGoingMask             = 0b00000100; // RxOnGoingMask
constexpr uint8_t SignalSynchronizedMask    = 0b00000010; // Signal Synchronized
constexpr uint8_t SignalDetectedMask        = 0b00000001; // Signal Detected

// RegPktSnrValue               0x19
constexpr uint8_t RegPktSnrValue            = 0x19;

double convertPktSnrValueToSNR(uint8_t pktSnr) {return -pktSnr/4;}

// RegPktRssiValue              0x1A
constexpr uint8_t RegPktRssiValue           = 0x1A; // RSSI of the latest packet received (dBm)

double convertPacketRssiValueToRssi(bool isHF, uint8_t pktRssi)
{
    if (isHF)
        return -157 + int(pktRssi);
    return -164 + int(pktRssi);
}

// RegRssiValue                 0x1B
constexpr uint8_t RegRssiValue              = 0x1B; // Current RSSI value (dBm)

double convertRssiValueToRssi(bool isHF, uint8_t rssi)
{
    if (isHF)
        return -157 + int(rssi);
    return -164 + int(rssi);
}

// RegHopChannel                0x1C
constexpr uint8_t RegHopChannel             = 0x1C;
constexpr uint8_t PllTimeoutMask            = 0b00000001; // PLL failed to lock while attempting a TX/RX/CAD operation
constexpr uint8_t CrcOnPayloadMask          = 0b00000001; // CRC Information extracted from the received packet header (Explicit header mode only)
constexpr uint8_t FhssPresentChannelMask    = 0b00011111; // Current value of frequency hopping channel in use


// RegModemConfig1              0x1D
constexpr uint8_t RegModemConfig1           = 0x1D;
constexpr uint8_t BwMask                    = 0b11110000; // Bw
constexpr uint8_t CodingRateMask            = 0b00001110; // CodingRate
constexpr uint8_t ImplicitHeaderModeOnMask  = 0b00000001; // ImplicitHeaderModeOn

enum Bw
{
    BW_7P8_KHZ,                  // 7.8 KHZ
    BW_10P4_KHZ,                 // 10.4 KHZ
    BW_15P6_KHZ,                 // 15.6 KHZ
    BW_20P8_KHZ,                 // 20.8 KHZ
    BW_31P25_KHZ,                // 31.25 KHZ
    BW_41P7_KHZ,                 // 41.7 KHZ
    BW_62P5_KHZ,                 // 62.5 KHZ
    BW_125_KHZ,                  // 125 KHZ
    BW_250_KHZ,                  // 250 KHZ
    BW_500_KHZ,                  // 500 KHZ
};

enum CodingRate
{
    CR_4V5,                      // 4/5
    CR_4V6,                      // 4/6
    CR_4V7,                      // 4/7
    CR_4V8                       // 4/8
};

// RegModemConfig2              0x1E
constexpr uint8_t RegModemConfig1           = 0x1E;
constexpr uint8_t SpreadngFactorMask        = 0b11110000; // SpreadngFactor
constexpr uint8_t TxContinousModeMask       = 0b00001000; // TxContinousMode
constexpr uint8_t RxPayloadCrcOnMask        = 0b00000100; // RxPayloadCrcOn
constexpr uint8_t SymbTimeoutMsbMask        = 0b00000011; // SymbTimeout MSB

enum SpreadngFactor
{
    SF_6 = 6,                   // SF6
    SF_7,                       // SF7
    SF_8,                       // SF8
    SF_9,                       // SF9
    SF_10,                      // SF10
    SF_11,                      // SF11
    SF_12                       // SF12
};

// RegSymbTimeoutLsb              0x1E
constexpr uint8_t RegSymbTimeoutLsb         = 0x1F; // SymbTimeout MSB

inline double convertSymbTimeoutToTimeout(double ts, uint16_t symbTimeout)
{
    return ts * symbTimeout;
}

// RegPreambleMsb               0x20
constexpr uint8_t RegPreambleMsb            = 0x20; // Preamble Length MSB
// RegPreambleLsb               0x21
constexpr uint8_t RegPreambleLsb            = 0x21; // Preamble Length LSB
// RegPayloadLength             0x22
constexpr uint8_t RegPayloadLength          = 0x22; // Payload Length
// RegMaxPayloadLength          0x23
constexpr uint8_t RegMaxPayloadLength       = 0x23; // Max Payload Length
// RegHopPeriod                 0x24
constexpr uint8_t RegHopPeriod              = 0x24; // Frequency Hopping Period
// RegFifoRxByteAddr            0x25
constexpr uint8_t RegFifoRxByteAddr         = 0x25; // Current value of RX databuffer pointe


// RegModemConfig3              0x26
constexpr uint8_t RegModemConfig3           = 0x26;
constexpr uint8_t LowDataRateOptimizeMask   = 0b00001000; // LowDataRateOptimize
constexpr uint8_t AgcAutoOnMask             = 0b00000100; // AgcAutoOn

// RegPpmCorrection             0x27
constexpr uint8_t RegPpmCorrection          = 0x27; // Data rate offset value, used in conjunction with AFC

// RegFeiMsb                    0x28
constexpr uint8_t RegFeiMsb                 = 0x28;
constexpr uint8_t FeiMsbMask                = 0b00001111; // FrequencyErrpr MSB

// RegFeiMid                    0x29
constexpr uint8_t RegFeiMid                 = 0x29; // FrequencyErrpr MID
// RegFeiLsb                    0x2A
constexpr uint8_t RegFeiLsb                 = 0x2A; // FrequencyErrpr LSB

inline double convertFreqErrorToFError(uint32_t freqError, uint32_t bw, uint32_t fxtal) {return (freqError*16777216*bw)/(fxtal*500);}

// RegRssiWideband              0x2C
constexpr uint8_t RegRssiWideband           = 0x2C; // Wideband RSSI measurement used to locally generate a random number

// RegDetectOptimize            0x31
constexpr uint8_t RegDetectOptimize         = 0x31;
constexpr uint8_t DetectOptimizeMask        = 0x00000011;

enum DetectOptimize             // LoRa Detection Optimize
{
    SF_6 = 5,                   // SF6
    SF_7 = 3,                   // SF7
    SF_8 = 3,                   // SF8
    SF_9 = 3,                   // SF9
    SF_10 = 3,                  // SF10
    SF_11 = 3,                  // SF11
    SF_12 = 3                   // SF12
};

// RegInvertIQ                  0x33
constexpr uint8_t RegInvertIQ               = 0x33;
constexpr uint8_t InvertIQmASK              = 0x01000000; // Invert the LoRa I and Q signals


// RegDetectionThreshold        0x37
constexpr uint8_t RegDetectionThreshold     = 0x37;

enum DetectionThreshold         // LoRa detection threshold
{
    SF_6 = 0xC,                 // SF6
    SF_7 = 0xA,                 // SF7
    SF_8 = 0xA,                 // SF8
    SF_9 = 0xA,                 // SF9
    SF_10 = 0xA,                // SF10
    SF_11 = 0xA,                // SF11
    SF_12 = 0xA                 // SF12
};

// RegSyncWord                  0x39
constexpr uint8_t RegSyncWord               = 0x39; // LoRa Sync Word

// RegDioMapping1               0x40
constexpr uint8_t RegDioMapping1            = 0x40; // DIO Mapping for DIO3:DIO0

enum DioMapping1
{
    CadDone_FhssChangeChannel_RxTimeout_RxDone,
    ValidHeader_FhssChangeChannel_FhssChangeChannel_TxDone,
    PayloadCrcError_FhssChangeChannel_CadDetected_CadDone
};

// RegDioMapping2               0x41
constexpr uint8_t RegDioMapping2            = 0x41; // DIO Mapping for DIO5:DIO4

enum DioMapping1
{
    ModeReady_CadDetected,
    ClkOut_PllLock,
    ClkOut_PllLock_2,
}

// RegVersion                   0x42
constexpr uint8_t RegVersion                = 0x42; // Version

// RegPllHop                    0x42
constexpr uint8_t RegPllHop                 = 0x42;
constexpr uint8_t FastHopOnMask             = 0b10000000; // FastHopOn

// RegTcxo                      0x4B
constexpr uint8_t RegTcxo                   = 0x4B;
constexpr uint8_t TcxoInputOnMask           = 0b00001000;

// RegPaDac                     0x4D
constexpr uint8_t RegPaDac                  = 0x4D;
constexpr uint8_t PaDacMask                 = 0b00000011; // PaDac
enum PaDac
{
    DEFAULT = 4,
    PA_BOOST = 7
};

// RegFormerTemp                0x5B
constexpr uint8_t RegFormerTemp             = 0x5B; // LoRa Sync Word

// RegAgcRef                    0x61
constexpr uint8_t RegAgcRef                 = 0x61;
constexpr uint8_t AgcReferenceLevelMask     = 0b00111111;

// RegAgcThresh1                0x62
constexpr uint8_t RegAgcThresh1             = 0x62; // Defines the 1st AGC Threshold
// RegAgcThresh2                0x63
constexpr uint8_t RegAgcThresh2             = 0x63; // Defines the 2nd AGC Threshold
// RegAgcThresh3                0x64
constexpr uint8_t RegAgcThresh3             = 0x64; // Defines the 3rd AGC Threshold

//RegPll                        0x70
constexpr uint8_t RegPll                    = 0x70;
constexpr uint8_t PllBandwidthMask          = 0b11000000;
enum PllBandwidth
{
    E_76KHZ,
    E_150KHZ,
    E_255KHZ,
    E_300KHZ
};

inline uint64_t getUnmasked(uint64_t mask, uint64_t value)
{
    for (; mask^1; mask = (mask>>1), value = (value>>1));
    return mask&value;
}

inline uint64_t setMasked(uint64_t mask, uint64_t value)
{
    auto omask = mask;
    for (; mask^1; mask = (mask>>1), value = (value<<1));
    return value&omask;
}
