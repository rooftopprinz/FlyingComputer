#ifndef __FLYDBCLIENT_HPP__
#define __FLYDBCLIENT_HPP__

#include <Buffer.hpp>
#include <protocol.hpp>

void createSetRequest(uint8_t pTrId, common::Buffer& pMsg)
{
    flydb::SetRequest request;
    request.header.msgType = flydb::MessageType::SetRequest;
    request.header.trId = pTrId;
}

flydb::Header getHeader(const common::Buffer& pMsg)
{
    flydb::Header rv;
    std::memcpy(&rv, pMsg.data(), sizeof(rv));
    return rv;
}

#endif // __FLYDBCLIENT_HPP__