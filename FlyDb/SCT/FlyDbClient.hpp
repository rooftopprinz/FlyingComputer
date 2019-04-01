#ifndef __FLYDBCLIENT_HPP__
#define __FLYDBCLIENT_HPP__

#include <Buffer.hpp>
#include <protocol.hpp>

template<typename T>
ssize_t createSetRequest(uint8_t pTrId, uint8_t pKey, const T& pValue, common::Buffer& pMsg)
{
    flydb::SetRequest request;
    request.header.msgType = flydb::MessageType::SetRequest;
    request.header.trId = pTrId;
    request.key = pKey;
    request.size = sizeof(T);
    std::memcpy(pMsg.data(), &request, sizeof(request));
    std::memcpy(pMsg.data() + sizeof(request), &pValue, sizeof(T));
    return sizeof(request) + sizeof(T);
}

flydb::Header getHeader(const common::Buffer& pMsg)
{
    flydb::Header rv;
    std::memcpy(&rv, pMsg.data(), sizeof(rv));
    return rv;
}

template<typename T>
T getDataFromGetResponse(const common::Buffer& pMsg)
{
    T rv;
    std::memcpy(&rv, pMsg.data() + sizeof(flydb::GetResponse), sizeof(rv));
    return rv;
}

#endif // __FLYDBCLIENT_HPP__