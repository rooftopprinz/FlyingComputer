// Type:  ('U8', {'type': 'unsigned'})
// Type:  ('U8', {'width': '8'})
// Type:  ('U16', {'type': 'unsigned'})
// Type:  ('U16', {'width': '16'})
// Type:  ('U32', {'type': 'unsigned'})
// Type:  ('U32', {'width': '32'})
// Type:  ('U64', {'type': 'unsigned'})
// Type:  ('U64', {'width': '64'})
// Type:  ('I8', {'type': 'signed'})
// Type:  ('I8', {'width': '8'})
// Type:  ('I16', {'type': 'signed'})
// Type:  ('I16', {'width': '16'})
// Type:  ('I32', {'type': 'signed'})
// Type:  ('I32', {'width': '32'})
// Type:  ('I64', {'type': 'signed'})
// Type:  ('I64', {'width': '64'})
// Type:  ('F32', {'type': 'float'})
// Type:  ('F64', {'type': 'double'})
// Type:  ('ParamId', {'type': 'unsigned'})
// Type:  ('ParamId', {'width': '8'})
// Sequence:  NoChoice ('U8', 'spare')
// Choice:  ('ParamData', 'NoChoice')
// Choice:  ('ParamData', 'U8')
// Choice:  ('ParamData', 'U16')
// Choice:  ('ParamData', 'U32')
// Choice:  ('ParamData', 'U64')
// Choice:  ('ParamData', 'I8')
// Choice:  ('ParamData', 'I16')
// Choice:  ('ParamData', 'I32')
// Choice:  ('ParamData', 'I64')
// Choice:  ('ParamData', 'F32')
// Choice:  ('ParamData', 'F64')
// Sequence:  ParamIdData ('U8', 'id')
// Sequence:  ParamIdData ('ParamData', 'data')
// Type:  ('ParamIdArray', {'type': 'ParamId'})
// Type:  ('ParamIdArray', {'static_array': '16'})
// Type:  ('ParamDataArray', {'type': 'ParamData'})
// Type:  ('ParamDataArray', {'static_array': '16'})
// Type:  ('ParamIdDataArray', {'type': 'ParamIdData'})
// Type:  ('ParamIdDataArray', {'static_array': '16'})
// Sequence:  ReadRequest ('ParamIdArray', 'paramId')
// Sequence:  ReadResponse ('ParamDataArray', 'paramData')
// Sequence:  WriteIndication ('ParamIdDataArray', 'paramIdData')
// Sequence:  WriteRequest ('ParamIdDataArray', 'paramIdData')
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
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;
using I8 = int8_t;
using I16 = int16_t;
using I32 = int32_t;
using I64 = int64_t;
using F32 = float;
using F64 = double;
using ParamId = uint8_t;
struct NoChoice
{
    U8 spare;
};

using ParamData = std::variant<NoChoice,U8,U16,U32,U64,I8,I16,I32,I64,F32,F64>;
struct ParamIdData
{
    U8 id;
    ParamData data;
};

using ParamIdArray = cum::static_array<ParamId, 16>;
using ParamDataArray = cum::static_array<ParamData, 16>;
using ParamIdDataArray = cum::static_array<ParamIdData, 16>;
struct ReadRequest
{
    ParamIdArray paramId;
};

struct ReadResponse
{
    ParamDataArray paramData;
};

struct WriteIndication
{
    ParamIdDataArray paramIdData;
};

struct WriteRequest
{
    ParamIdDataArray paramIdData;
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

inline void encode_per(const NoChoice& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.spare, pCtx);
}

inline void decode_per(NoChoice& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.spare, pCtx);
}

inline void str(const char* pName, const NoChoice& pIe, std::string& pCtx, bool pIsLast)
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

inline void encode_per(const ParamData& pIe, cum::per_codec_ctx& pCtx)
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
}

inline void decode_per(ParamData& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    using TypeIndex = uint8_t;
    TypeIndex type;
    decode_per(type, pCtx);
    if (0 == type)
    {
        pIe = NoChoice{};
        decode_per(std::get<0>(pIe), pCtx);
    }
    else if (1 == type)
    {
        pIe = U8{};
        decode_per(std::get<1>(pIe), pCtx);
    }
    else if (2 == type)
    {
        pIe = U16{};
        decode_per(std::get<2>(pIe), pCtx);
    }
    else if (3 == type)
    {
        pIe = U32{};
        decode_per(std::get<3>(pIe), pCtx);
    }
    else if (4 == type)
    {
        pIe = U64{};
        decode_per(std::get<4>(pIe), pCtx);
    }
    else if (5 == type)
    {
        pIe = I8{};
        decode_per(std::get<5>(pIe), pCtx);
    }
    else if (6 == type)
    {
        pIe = I16{};
        decode_per(std::get<6>(pIe), pCtx);
    }
    else if (7 == type)
    {
        pIe = I32{};
        decode_per(std::get<7>(pIe), pCtx);
    }
    else if (8 == type)
    {
        pIe = I64{};
        decode_per(std::get<8>(pIe), pCtx);
    }
    else if (9 == type)
    {
        pIe = F32{};
        decode_per(std::get<9>(pIe), pCtx);
    }
    else if (10 == type)
    {
        pIe = F64{};
        decode_per(std::get<10>(pIe), pCtx);
    }
}

inline void str(const char* pName, const ParamData& pIe, std::string& pCtx, bool pIsLast)
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
    encode_per(pIe.paramId, 1, pCtx);
}

inline void decode_per(ReadRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramId, 1, pCtx);
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
    str("paramId", pIe.paramId, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

inline void encode_per(const ReadResponse& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramData, 1, pCtx);
}

inline void decode_per(ReadResponse& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramData, 1, pCtx);
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
    encode_per(pIe.paramIdData, 1, pCtx);
}

inline void decode_per(WriteIndication& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramIdData, 1, pCtx);
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
    str("paramIdData", pIe.paramIdData, pCtx, !(--nMandatory+nOptional));
    pCtx = pCtx + "}";
    if (!pIsLast)
    {
        pCtx += ",";
    }
}

inline void encode_per(const WriteRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    encode_per(pIe.paramIdData, 1, pCtx);
}

inline void decode_per(WriteRequest& pIe, cum::per_codec_ctx& pCtx)
{
    using namespace cum;
    decode_per(pIe.paramIdData, 1, pCtx);
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
    str("paramIdData", pIe.paramIdData, pCtx, !(--nMandatory+nOptional));
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
