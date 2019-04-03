#include <gtest/gtest.h>
#include <mock/MockISocket.hpp>
#include <DbServer.hpp>
#include <cstring>
#include <iostream>
#include <iomanip>

using namespace ::testing;

struct DbServerTest : public Test
{
    DbServerTest()
        : dbServer(mockISocket)
    {}

    MockISocket mockISocket;
    DbServer dbServer;
};

void print(const char* msg, const uint8_t* start, size_t size)
{
    std::cout << msg << " [" << size << "] = ";
    for (size_t i=0; i<size; i++)
    {
        std::cout << std::setw(2) << std::setfill('0') << std::hex << unsigned(start[i]) << " ";
    }
    std::cout << "\n";

}

bool isMessageEqual(const common::Buffer& a, const common::Buffer& b)
{
    return a.size() == b.size() && (std::memcmp(a.data(), b.data(), a.size())==0);
}

bool isIpPortEqual(const net::IpPort& a, const net::IpPort& b)
{
    return a.addr == b.addr && a.port == b.port;
}

TEST_F(DbServerTest, shouldHandleSetRequestAndGetRequest)
{
    std::byte requestrawbuffer[1024];
    std::byte response0rawbuffer[1024];
    // std::byte response1rawbuffer[1024];

    net::IpPort from(4, 1555);
    flydb::TrId trId = 0;

    flydb::Encoder<flydb::SetResponse> setResponseEncoder(response0rawbuffer, sizeof(response0rawbuffer));
    setResponseEncoder.get().trId = trId;
    common::Buffer toReceive0(response0rawbuffer, setResponseEncoder.size(), false);

    uint8_t getResponseRaw[] = {
        (uint8_t)flydb::MessageType::GetResponse,
        1,
        3 + 6*3,
        42, 4, 42, 0, 0, 0,
        50, 4, 50, 0, 0, 0,
        51, 4, 51, 0, 0, 0
    };

    common::Buffer toReceive1((std::byte*)getResponseRaw, 21, false);

    EXPECT_CALL(mockISocket, sendto(
        Truly([&toReceive0](const common::Buffer& a) {return isMessageEqual(a, toReceive0);}),
        Truly([&from](const net::IpPort& a) {return isIpPortEqual(a, from);})
        , 0)).RetiresOnSaturation();

    EXPECT_CALL(mockISocket, sendto(
        Truly([&toReceive1](const common::Buffer& a) {return isMessageEqual(a, toReceive1);}),
        Truly([&from](const net::IpPort& a) {return isIpPortEqual(a, from);})
        , 0)).RetiresOnSaturation();

    {
        flydb::Encoder<flydb::SetRequest> setRequestEncoder(requestrawbuffer, sizeof(requestrawbuffer));
        auto& request = setRequestEncoder.get();
        request.trId = 0;
        setRequestEncoder.addField(42, 42);
        setRequestEncoder.addField(50, 50);
        setRequestEncoder.addField(51, 51);
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