#include <gtest/gtest.h>
#include <mock/MockISocket.hpp>
#include <DbServer.hpp>
#include <cstring>

using namespace ::testing;

struct DbServerTest : public Test
{
    DbServerTest()
        : dbServer(mockISocket)
    {}

    MockISocket mockISocket;
    DbServer dbServer;
};

bool isMessageEqual(const common::Buffer& a, const common::Buffer& b)
{
    return a.size() == b.size() && std::memcmp(a.data(), b.data(), a.size())==0;
}

bool isIpPortEqual(const net::IpPort& a, const net::IpPort& b)
{
    return a.addr == b.addr && a.port == b.port;
}

TEST_F(DbServerTest, shouldHandleSetRequest)
{
    flydb::SetRequest setRequest;
    setRequest.header.msgType = flydb::MessageType::SetRequest;
    setRequest.header.trId = 0;
    setRequest.key = 1;
    float data = 4.2;
    std::byte toSendRaw[sizeof(setRequest)+sizeof(data)];
    std::memcpy(toSendRaw, &setRequest, sizeof(setRequest));
    std::memcpy(toSendRaw+sizeof(setRequest), &data, sizeof(data));
    common::Buffer toSend(toSendRaw, sizeof(toSendRaw), false);

    flydb::SetResponse setResponse;
    setResponse.header.msgType = flydb::MessageType::SetResponse;
    setResponse.header.trId = 0;
    common::Buffer toReceive((std::byte*)&setResponse, sizeof(setResponse), false);

    net::IpPort from(4, 1555);

    EXPECT_CALL(mockISocket, sendto(
        Truly([&toReceive](const common::Buffer& a) {return isMessageEqual(a, toReceive);}),
        Truly([&from](const net::IpPort& a) {return isIpPortEqual(a, from);})
        , 0));

    dbServer.onReceive(toSend, from);
}