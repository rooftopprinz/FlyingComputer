#ifndef __FLYINTERFACE_PROTOCOL_HPP__
#define __FLYINTERFACE_PROTOCOL_HPP__

#include <cstdint>

namespace flydb
{

enum class MessageType : uint8_t
{
    SetRequest,
    SetResponse,
    SetIndication,
    GetRequest,
    GetResponse
};

struct Header
{
    MessageType msgType;
    uint8_t trId;
};

struct SetRequest
{
    Header header;
    uint8_t key;
    uint8_t size;
};

struct SetResponse
{
    Header header;
};

struct SetIndication
{
    Header header;
    uint8_t key;
    uint8_t size;
};

struct GetRequest
{
    Header header;
    uint8_t key;
};

struct GetResponse
{
    Header header;
    uint8_t size;
};

};

#endif // __FLYINTERFACE_PROTOCOL_HPP__
