// Type:  ('Buffer', {'type': 'unsigned'})
// Type:  ('Buffer', {'width': '8'})
// Type:  ('Buffer', {'dynamic_array': '256'})
// Type:  ('ParamId', {'type': 'unsigned'})
// Type:  ('ParamId', {'width': '8'})
// Sequence:  ParamInfo ('ParamId', 'paramId')
// Sequence:  ParamInfo ('Buffer', 'paramData')
// Sequence:  ReadRequest ('ParamId', 'paramId')
// Sequence:  ReadGroupRequest ('ParamId', 'paramId')
// Sequence:  ReadResponse ('SN', 'reqSn')
// Sequence:  ReadResponse ('Buffer', 'paramData')
// Sequence:  WriteRequest ('ParamId', 'paramId')
// Sequence:  WriteRequest ('Buffer', 'paramData')
// Sequence:  WriteGroupRequest ('ParamId', 'paramId')
// Sequence:  WriteGroupRequest ('Buffer', 'paramData')
// Sequence:  WriteResponse ('SN', 'reqSn')
// Sequence:  ReadIndication ('ParamId', 'paramId')
// Sequence:  ReadIndication ('Buffer', 'paramData')
// Sequence:  ReadGroupIndication ('ParamId', 'paramId')
// Sequence:  ReadGroupIndication ('Buffer', 'paramData')
// Sequence:  WriteIndication ('ParamId', 'paramId')
// Sequence:  WriteIndication ('Buffer', 'paramData')
// Sequence:  WriteGroupIndication ('ParamId', 'paramId')
// Sequence:  WriteGroupIndication ('Buffer', 'paramData')
// Sequence:  ErrorIndication ('SN', 'reqSn')
// Sequence:  ErrorIndication ('Cause', 'cause')
// Sequence:  ProtectAndCipherRequest ('U64', 'random')
// Sequence:  ProtectAndCipherResponse ('U64', 'res')
// Choice:  ('MessageType', 'ReadRequest')
// Choice:  ('MessageType', 'ReadGroupRequest')
// Choice:  ('MessageType', 'ReadResponse')
// Choice:  ('MessageType', 'WriteRequest')
// Choice:  ('MessageType', 'WriteGroupRequest')
// Choice:  ('MessageType', 'WriteResponse')
// Choice:  ('MessageType', 'ReadIndication')
// Choice:  ('MessageType', 'ReadGroupIndication')
// Choice:  ('MessageType', 'WriteIndication')
// Choice:  ('MessageType', 'WriteGroupIndication')
// Choice:  ('MessageType', 'ErrorIndication')
// Choice:  ('MessageType', 'ProtectAndCipherRequest')
// Choice:  ('MessageType', 'ProtectAndCipherResponse')
// Type:  ('SN', {'type': 'unsigned'})
// Type:  ('SN', {'width': '8'})
// Sequence:  Message ('SN', 'sequenceNumber')
// Sequence:  Message ('MessageType', 'message')
// Type:  ('MAC', {'type': 'unsigned'})
// Type:  ('MAC', {'width': '16'})
// Type:  ('Buffer', {'type': 'unsigned'})
// Type:  ('Buffer', {'width': '8'})
// Type:  ('Buffer', {'dynamic_array': '256'})
// Sequence:  Itf ('MAC', 'msgAuthCode')
// Sequence:  Itf ('Buffer', 'encodedMessage')
// Generating for C++
#ifndef __CUM_MSG_HPP__
#define __CUM_MSG_HPP__
#include "cum/cum.hpp"
#include <optional>

/***********************************************
/
/            Message Definitions
/
************************************************/

using Buffer = std::vector<uint8_t>;
using ParamId = uint8_t;
struct ParamInfo
{
    ParamId paramId;
    Buffer paramData;
};

struct ReadRequest
{
    ParamId paramId;
};

struct ReadGroupRequest
{
    ParamId paramId;
};

struct ReadResponse
{
    SN reqSn;
    Buffer paramData;
};

struct WriteRequest
{
    ParamId paramId;
    Buffer paramData;
};

struct WriteGroupRequest
{
    ParamId paramId;
    Buffer paramData;
};

struct WriteResponse
{
    SN reqSn;
};

struct ReadIndication
{
    ParamId paramId;
    Buffer paramData;
};

struct ReadGroupIndication
{
    ParamId paramId;
    Buffer paramData;
};

struct WriteIndication
{
    ParamId paramId;
    Buffer paramData;
};

struct WriteGroupIndication
{
    ParamId paramId;
    Buffer paramData;
};

struct ErrorIndication
{
    SN reqSn;
    Cause cause;
};

struct ProtectAndCipherRequest
{
    U64 random;
};

struct ProtectAndCipherResponse
{
    U64 res;
};

using MessageType = std::variant<ReadRequest,ReadGroupRequest,ReadResponse,WriteRequest,WriteGroupRequest,WriteResponse,ReadIndication,ReadGroupIndication,WriteIndication,WriteGroupIndication,ErrorIndication,ProtectAndCipherRequest,ProtectAndCipherResponse>;
using SN = uint8_t;
struct Message
{
    SN sequenceNumber;
    MessageType message;
};

using MAC = uint16_t;
using Buffer = std::vector<uint8_t>;
struct Itf
{
    MAC msgAuthCode;
    Buffer encodedMessage;
};

/***********************************************
/
/            Codec Definitions
/
************************************************/

void encode_per(const ParamInfo& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramId, pCtx);
    encode_per(pIe.paramData, 2, pCtx);
}

void decode_per(ParamInfo& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramId, pCtx);
    decode_per(pIe.paramData, 2, pCtx);
}

void str(const char* pName, const ParamInfo& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 2;
    str("paramId", pIe.paramId, pCtx, !(--nMandatory+nOptional));
    str("paramData", pIe.paramData, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const ReadRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramId, pCtx);
}

void decode_per(ReadRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramId, pCtx);
}

void str(const char* pName, const ReadRequest& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 1;
    str("paramId", pIe.paramId, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const ReadGroupRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramId, pCtx);
}

void decode_per(ReadGroupRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramId, pCtx);
}

void str(const char* pName, const ReadGroupRequest& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 1;
    str("paramId", pIe.paramId, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const ReadResponse& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.reqSn, pCtx);
    encode_per(pIe.paramData, 2, pCtx);
}

void decode_per(ReadResponse& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.reqSn, pCtx);
    decode_per(pIe.paramData, 2, pCtx);
}

void str(const char* pName, const ReadResponse& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 2;
    str("reqSn", pIe.reqSn, pCtx, !(--nMandatory+nOptional));
    str("paramData", pIe.paramData, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const WriteRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramId, pCtx);
    encode_per(pIe.paramData, 2, pCtx);
}

void decode_per(WriteRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramId, pCtx);
    decode_per(pIe.paramData, 2, pCtx);
}

void str(const char* pName, const WriteRequest& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 2;
    str("paramId", pIe.paramId, pCtx, !(--nMandatory+nOptional));
    str("paramData", pIe.paramData, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const WriteGroupRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramId, pCtx);
    encode_per(pIe.paramData, 2, pCtx);
}

void decode_per(WriteGroupRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramId, pCtx);
    decode_per(pIe.paramData, 2, pCtx);
}

void str(const char* pName, const WriteGroupRequest& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 2;
    str("paramId", pIe.paramId, pCtx, !(--nMandatory+nOptional));
    str("paramData", pIe.paramData, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const WriteResponse& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.reqSn, pCtx);
}

void decode_per(WriteResponse& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.reqSn, pCtx);
}

void str(const char* pName, const WriteResponse& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 1;
    str("reqSn", pIe.reqSn, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const ReadIndication& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramId, pCtx);
    encode_per(pIe.paramData, 2, pCtx);
}

void decode_per(ReadIndication& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramId, pCtx);
    decode_per(pIe.paramData, 2, pCtx);
}

void str(const char* pName, const ReadIndication& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 2;
    str("paramId", pIe.paramId, pCtx, !(--nMandatory+nOptional));
    str("paramData", pIe.paramData, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const ReadGroupIndication& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramId, pCtx);
    encode_per(pIe.paramData, 2, pCtx);
}

void decode_per(ReadGroupIndication& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramId, pCtx);
    decode_per(pIe.paramData, 2, pCtx);
}

void str(const char* pName, const ReadGroupIndication& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 2;
    str("paramId", pIe.paramId, pCtx, !(--nMandatory+nOptional));
    str("paramData", pIe.paramData, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const WriteIndication& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramId, pCtx);
    encode_per(pIe.paramData, 2, pCtx);
}

void decode_per(WriteIndication& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramId, pCtx);
    decode_per(pIe.paramData, 2, pCtx);
}

void str(const char* pName, const WriteIndication& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 2;
    str("paramId", pIe.paramId, pCtx, !(--nMandatory+nOptional));
    str("paramData", pIe.paramData, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const WriteGroupIndication& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramId, pCtx);
    encode_per(pIe.paramData, 2, pCtx);
}

void decode_per(WriteGroupIndication& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramId, pCtx);
    decode_per(pIe.paramData, 2, pCtx);
}

void str(const char* pName, const WriteGroupIndication& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 2;
    str("paramId", pIe.paramId, pCtx, !(--nMandatory+nOptional));
    str("paramData", pIe.paramData, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const ErrorIndication& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.reqSn, pCtx);
    encode_per(pIe.cause, pCtx);
}

void decode_per(ErrorIndication& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.reqSn, pCtx);
    decode_per(pIe.cause, pCtx);
}

void str(const char* pName, const ErrorIndication& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 2;
    str("reqSn", pIe.reqSn, pCtx, !(--nMandatory+nOptional));
    str("cause", pIe.cause, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const ProtectAndCipherRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.random, pCtx);
}

void decode_per(ProtectAndCipherRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.random, pCtx);
}

void str(const char* pName, const ProtectAndCipherRequest& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 1;
    str("random", pIe.random, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const ProtectAndCipherResponse& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.res, pCtx);
}

void decode_per(ProtectAndCipherResponse& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.res, pCtx);
}

void str(const char* pName, const ProtectAndCipherResponse& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 1;
    str("res", pIe.res, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const MessageType& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    using TypeIndex = uint8_t;
    TypeIndex type = pIe.index();
    encode_per(type, pCtx);
    if (0 == type)
    {
        encode_per(std::get<0>(pIe), pCtx);
    }
    else if (1 == type)
    {
        encode_per(std::get<1>(pIe), pCtx);
    }
    else if (2 == type)
    {
        encode_per(std::get<2>(pIe), pCtx);
    }
    else if (3 == type)
    {
        encode_per(std::get<3>(pIe), pCtx);
    }
    else if (4 == type)
    {
        encode_per(std::get<4>(pIe), pCtx);
    }
    else if (5 == type)
    {
        encode_per(std::get<5>(pIe), pCtx);
    }
    else if (6 == type)
    {
        encode_per(std::get<6>(pIe), pCtx);
    }
    else if (7 == type)
    {
        encode_per(std::get<7>(pIe), pCtx);
    }
    else if (8 == type)
    {
        encode_per(std::get<8>(pIe), pCtx);
    }
    else if (9 == type)
    {
        encode_per(std::get<9>(pIe), pCtx);
    }
    else if (10 == type)
    {
        encode_per(std::get<10>(pIe), pCtx);
    }
    else if (11 == type)
    {
        encode_per(std::get<11>(pIe), pCtx);
    }
    else if (12 == type)
    {
        encode_per(std::get<12>(pIe), pCtx);
    }
}

void decode_per(MessageType& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    using TypeIndex = uint8_t;
    TypeIndex type;
    decode_per(type, pCtx);
    if (0 == type)
    {
        pIe = ReadRequest{};
        decode_per(std::get<0>(pIe), pCtx);
    }
    else if (1 == type)
    {
        pIe = ReadGroupRequest{};
        decode_per(std::get<1>(pIe), pCtx);
    }
    else if (2 == type)
    {
        pIe = ReadResponse{};
        decode_per(std::get<2>(pIe), pCtx);
    }
    else if (3 == type)
    {
        pIe = WriteRequest{};
        decode_per(std::get<3>(pIe), pCtx);
    }
    else if (4 == type)
    {
        pIe = WriteGroupRequest{};
        decode_per(std::get<4>(pIe), pCtx);
    }
    else if (5 == type)
    {
        pIe = WriteResponse{};
        decode_per(std::get<5>(pIe), pCtx);
    }
    else if (6 == type)
    {
        pIe = ReadIndication{};
        decode_per(std::get<6>(pIe), pCtx);
    }
    else if (7 == type)
    {
        pIe = ReadGroupIndication{};
        decode_per(std::get<7>(pIe), pCtx);
    }
    else if (8 == type)
    {
        pIe = WriteIndication{};
        decode_per(std::get<8>(pIe), pCtx);
    }
    else if (9 == type)
    {
        pIe = WriteGroupIndication{};
        decode_per(std::get<9>(pIe), pCtx);
    }
    else if (10 == type)
    {
        pIe = ErrorIndication{};
        decode_per(std::get<10>(pIe), pCtx);
    }
    else if (11 == type)
    {
        pIe = ProtectAndCipherRequest{};
        decode_per(std::get<11>(pIe), pCtx);
    }
    else if (12 == type)
    {
        pIe = ProtectAndCipherResponse{};
        decode_per(std::get<12>(pIe), pCtx);
    }
}

void str(const char* pName, const MessageType& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    using TypeIndex = uint8_t;
    TypeIndex type = pIe.index();
    if (0 == type)
    {
        str(pName, std::get<0>(pIe), pCtx, true);
    }
    else if (1 == type)
    {
        str(pName, std::get<1>(pIe), pCtx, true);
    }
    else if (2 == type)
    {
        str(pName, std::get<2>(pIe), pCtx, true);
    }
    else if (3 == type)
    {
        str(pName, std::get<3>(pIe), pCtx, true);
    }
    else if (4 == type)
    {
        str(pName, std::get<4>(pIe), pCtx, true);
    }
    else if (5 == type)
    {
        str(pName, std::get<5>(pIe), pCtx, true);
    }
    else if (6 == type)
    {
        str(pName, std::get<6>(pIe), pCtx, true);
    }
    else if (7 == type)
    {
        str(pName, std::get<7>(pIe), pCtx, true);
    }
    else if (8 == type)
    {
        str(pName, std::get<8>(pIe), pCtx, true);
    }
    else if (9 == type)
    {
        str(pName, std::get<9>(pIe), pCtx, true);
    }
    else if (10 == type)
    {
        str(pName, std::get<10>(pIe), pCtx, true);
    }
    else if (11 == type)
    {
        str(pName, std::get<11>(pIe), pCtx, true);
    }
    else if (12 == type)
    {
        str(pName, std::get<12>(pIe), pCtx, true);
    }
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const Message& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.sequenceNumber, pCtx);
    encode_per(pIe.message, pCtx);
}

void decode_per(Message& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.sequenceNumber, pCtx);
    decode_per(pIe.message, pCtx);
}

void str(const char* pName, const Message& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 2;
    str("sequenceNumber", pIe.sequenceNumber, pCtx, !(--nMandatory+nOptional));
    str("message", pIe.message, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

void encode_per(const Itf& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.msgAuthCode, pCtx);
    encode_per(pIe.encodedMessage, 2, pCtx);
}

void decode_per(Itf& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.msgAuthCode, pCtx);
    decode_per(pIe.encodedMessage, 2, pCtx);
}

void str(const char* pName, const Itf& pIe, std::string& pCtx, bool pIsLast)
{
    using namespace cum;
    if (!pName)
    {
        pCtx = pCtx + "{";
    }
    else
    {
        pCtx = pCtx + "\"" + pName + "\":{";
    }
    size_t nOptional = 0;
    size_t nMandatory = 2;
    str("msgAuthCode", pIe.msgAuthCode, pCtx, !(--nMandatory+nOptional));
    str("encodedMessage", pIe.encodedMessage, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

#endif //__CUM_MSG_HPP__
