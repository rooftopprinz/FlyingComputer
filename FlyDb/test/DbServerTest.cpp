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

TEST_F(DbServerTest, shouldHandleSetRequestAndGetRequest)
{
    std::byte requestrawbuffer[1024];
    std::byte response0rawbuffer[1024];
    std::byte response1rawbuffer[1024];

    net::IpPort from(4, 1555);


    flydb::Encoder<flydb::SetResponse> setResponseEncoder(response0rawbuffer, sizeof(response0rawbuffer));
    setResponseEncoder.get().trId = 0;
    common::Buffer toReceive0(response0rawbuffer, setResponseEncoder.size(), false);

    flydb::Encoder<flydb::GetResponse> getResponseEncoder(response1rawbuffer, sizeof(response1rawbuffer));
    getResponseEncoder.get().trId = 1;
    getResponseEncoder.addField(42, 4.2);
    getResponseEncoder.addField(50, 5.0);
    getResponseEncoder.addField(51, 5.1);
    common::Buffer toReceive1(response1rawbuffer, getResponseEncoder.size(), false);

    EXPECT_CALL(mockISocket, sendto(
        Truly([&toReceive0](const common::Buffer& a) {return isMessageEqual(a, toReceive0);}),
        Truly([&from](const net::IpPort& a) {return isIpPortEqual(a, from);})
        , 0));

    EXPECT_CALL(mockISocket, sendto(
        Truly([&toReceive1](const common::Buffer& a) {return isMessageEqual(a, toReceive1);}),
        Truly([&from](const net::IpPort& a) {return isIpPortEqual(a, from);})
        , 0));

    {
        flydb::Encoder<flydb::SetRequest> setRequestEncoder(requestrawbuffer, sizeof(requestrawbuffer));
        auto& request = setRequestEncoder.get();
        request.trId = 0;
        setRequestEncoder.addField(42, 4.2);
        setRequestEncoder.addField(50, 5.0);
        setRequestEncoder.addField(51, 5.1);
        dbServer.onReceive(common::Buffer(requestrawbuffer, setRequestEncoder.size(), false), from);
    }

    {
        flydb::Encoder<flydb::GetRequest> getRequestEncoder(requestrawbuffer, sizeof(requestrawbuffer));
        auto& request = getRequestEncoder.get();
        request.trId = 1;
        getRequestEncoder.addField((flydb::Size)42);
        getRequestEncoder.addField((flydb::Size)50);
        getRequestEncoder.addField((flydb::Size)51);
        dbServer.onReceive(common::Buffer(requestrawbuffer, getRequestEncoder.size(), false), from);
    }
}