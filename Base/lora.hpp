#include "lora_itf.hpp"

class Sx127xFlyLora
{
public:
    Sx127xFlyLora(ISpi& pSpi, IGpio& pGpio)
        : mSpi(pSpi)
        , mGpio(pGpio)
    {
        // Reset module
    }

    void setUsage(); // TX, RX CONTINUOUS
    void setCarrier(); // Carrier frequency

    void setSf(); // Spreading Factor 
    void setBw(); // Bandwidth
    void setCodingRate(); // Coding Rate
    void setHeaderMode(); // Explicit or implicit

    void setPaSelect(); // +14 dbm or +20 dbm
    void setMaxPower();
    void setOutputPower(); //

    void setLnaGain();

    void getLastRssi();

    void tx(); // transmit
    void rx(); // receive

private:
    void onRxDone();
    void onTxDone();

    ISpi& mSpi;
    IGpio& mGpio;
};