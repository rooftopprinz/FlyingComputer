#ifndef __FLYINTERFACE_PROTOCOL_HPP__
#define __FLYINTERFACE_PROTOCOL_HPP__

namespace flydb
{

struct Header
{
    uint8_t msgType;
    uint8_t trId;
};

struct GetRequest
{
    Header header;
    uint8_t key;
};

struct GetResponse
{
    Header header;
    uint8_t data[0];
};

struct SetRequest
{
    Header header;
    uint8_t key;
    uint8_t data[0];
};

struct SetResponse
{
    Header header;
};

setruct SetIndication
{
    Header header;
    uint8_t key;
    uint8_t data[0];
};

struct SubsribeRequest
{
    Header header;
    uint8_t key;
};

struct SubsribeResponse
{
    Header header;
    uint16_t id;
};
 
};

#endif // __FLYINTERFACE_PROTOCOL_HPP__
