// Type:  ('U8', {'type': 'unsigned'})
// Type:  ('U8', {'width': '8'})
// Type:  ('Buffer', {'type': 'unsigned'})
// Type:  ('Buffer', {'dynamic_array': '255'})
// Type:  ('Buffer', {'width': '8'})
// Sequence:  ParamData ('Buffer', 'value')
// Sequence:  ParamIdData ('U8', 'id')
// Sequence:  ParamIdData ('ParamData', 'data')
// Type:  ('ParamIdArray', {'type': 'unsigned'})
// Type:  ('ParamIdArray', {'dynamic_array': '256'})
// Type:  ('ParamIdArray', {'width': '8'})
// Type:  ('ParamDataArray', {'type': 'ParamData'})
// Type:  ('ParamDataArray', {'dynamic_array': '256'})
// Type:  ('ParamIdDataArray', {'type': 'ParamIdData'})
// Type:  ('ParamIdDataArray', {'dynamic_array': '256'})
// Sequence:  ReadRequest ('ParamIdArray', 'paramIds')
// Sequence:  ReadResponse ('ParamDataArray', 'paramData')
// Sequence:  WriteIndication ('ParamIdDataArray', 'paramIds')
// Sequence:  WriteRequest ('ParamIdDataArray', 'paramIds')
// Sequence:  WriteResponse ('U8', 'spare')
// Choice:  ('Message', 'ReadRequest')
// Choice:  ('Message', 'ReadResponse')
// Choice:  ('Message', 'WriteIndication')
// Choice:  ('Message', 'WriteRequest')
// Choice:  ('Message', 'WriteResponse')
// Sequence:  FlyDbMessage ('U8', 'transactionId')
// Sequence:  FlyDbMessage ('Message', 'msg')
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

using U8 = uint8_t;
using Buffer = std::vector<uint8_t>;
struct ParamData
{
    Buffer value;
};

struct ParamIdData
{
    U8 id;
    ParamData data;
};

using ParamIdArray = std::vector<uint8_t>;
using ParamDataArray = std::vector<ParamData>;
using ParamIdDataArray = std::vector<ParamIdData>;
struct ReadRequest
{
    ParamIdArray paramIds;
};

struct ReadResponse
{
    ParamDataArray paramData;
};

struct WriteIndication
{
    ParamIdDataArray paramIds;
};

struct WriteRequest
{
    ParamIdDataArray paramIds;
};

struct WriteResponse
{
    U8 spare;
};

using Message = std::variant<ReadRequest,ReadResponse,WriteIndication,WriteRequest,WriteResponse>;
struct FlyDbMessage
{
    U8 transactionId;
    Message msg;
};

/***********************************************
/
/            Codec Definitions
/
************************************************/

inline void encode_per(const ParamData& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.value, 1, pCtx);
}

inline void decode_per(ParamData& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.value, 1, pCtx);
}

inline void str(const char* pName, const ParamData& pIe, std::string& pCtx, bool pIsLast)
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
    str("value", pIe.value, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

inline void encode_per(const ParamIdData& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.id, pCtx);
    encode_per(pIe.data, pCtx);
}

inline void decode_per(ParamIdData& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.id, pCtx);
    decode_per(pIe.data, pCtx);
}

inline void str(const char* pName, const ParamIdData& pIe, std::string& pCtx, bool pIsLast)
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
    str("id", pIe.id, pCtx, !(--nMandatory+nOptional));
    str("data", pIe.data, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

inline void encode_per(const ReadRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramIds, 2, pCtx);
}

inline void decode_per(ReadRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramIds, 2, pCtx);
}

inline void str(const char* pName, const ReadRequest& pIe, std::string& pCtx, bool pIsLast)
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
    str("paramIds", pIe.paramIds, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

inline void encode_per(const ReadResponse& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramData, 2, pCtx);
}

inline void decode_per(ReadResponse& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramData, 2, pCtx);
}

inline void str(const char* pName, const ReadResponse& pIe, std::string& pCtx, bool pIsLast)
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
    str("paramData", pIe.paramData, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

inline void encode_per(const WriteIndication& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramIds, 2, pCtx);
}

inline void decode_per(WriteIndication& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramIds, 2, pCtx);
}

inline void str(const char* pName, const WriteIndication& pIe, std::string& pCtx, bool pIsLast)
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
    str("paramIds", pIe.paramIds, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

inline void encode_per(const WriteRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramIds, 2, pCtx);
}

inline void decode_per(WriteRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramIds, 2, pCtx);
}

inline void str(const char* pName, const WriteRequest& pIe, std::string& pCtx, bool pIsLast)
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
    str("paramIds", pIe.paramIds, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

inline void encode_per(const WriteResponse& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.spare, pCtx);
}

inline void decode_per(WriteResponse& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.spare, pCtx);
}

inline void str(const char* pName, const WriteResponse& pIe, std::string& pCtx, bool pIsLast)
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
    str("spare", pIe.spare, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

inline void encode_per(const Message& pIe, cum::per_codec_ctx& pCtx)
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
}

inline void decode_per(Message& pIe, cum::per_codec_ctx& pCtx)
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
        pIe = ReadResponse{};
        decode_per(std::get<1>(pIe), pCtx);
    }
    else if (2 == type)
    {
        pIe = WriteIndication{};
        decode_per(std::get<2>(pIe), pCtx);
    }
    else if (3 == type)
    {
        pIe = WriteRequest{};
        decode_per(std::get<3>(pIe), pCtx);
    }
    else if (4 == type)
    {
        pIe = WriteResponse{};
        decode_per(std::get<4>(pIe), pCtx);
    }
}

inline void str(const char* pName, const Message& pIe, std::string& pCtx, bool pIsLast)
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
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

inline void encode_per(const FlyDbMessage& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.transactionId, pCtx);
    encode_per(pIe.msg, pCtx);
}

inline void decode_per(FlyDbMessage& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.transactionId, pCtx);
    decode_per(pIe.msg, pCtx);
}

inline void str(const char* pName, const FlyDbMessage& pIe, std::string& pCtx, bool pIsLast)
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
    str("transactionId", pIe.transactionId, pCtx, !(--nMandatory+nOptional));
    str("msg", pIe.msg, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

#endif //__CUM_MSG_HPP__
