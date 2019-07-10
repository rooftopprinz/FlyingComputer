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
    std::byte response1rawbuffer[1024];

    net::IpPort from(4, 1555);

    FlyDbMessage writeReponseMsg;
    writeReponseMsg.msg = WriteResponse{};
    auto& writeReponse = std::get<WriteResponse>(writeReponseMsg.msg);
    writeReponseMsg.transactionId = 0;
    writeReponse.spare = 0;

    cum::per_codec_ctx writeReponseCtx(response0rawbuffer, sizeof(response0rawbuffer));
    encode_per(writeReponseMsg, writeReponseCtx);
    auto writeReponseEncodeSize = sizeof(response0rawbuffer) - writeReponseCtx.size();

    common::Buffer toReceive0(response0rawbuffer, writeReponseEncodeSize, false);

    FlyDbMessage readResponseMsg;
    readResponseMsg.msg = ReadResponse{};
    readResponseMsg.transactionId = 1;
    auto& readResponse = std::get<ReadResponse>(readResponseMsg.msg);
    readResponse.paramData.emplace_back(ParamData{{42,0,0,0}});
    readResponse.paramData.emplace_back(ParamData{{43,0}});
    readResponse.paramData.emplace_back(ParamData{{44,0,0}});
    readResponse.paramData.emplace_back(ParamData{{45,0,0,0,0,0,0,0}});

    cum::per_codec_ctx readResponseCtx(response1rawbuffer, sizeof(response1rawbuffer));
    encode_per(readResponseMsg, readResponseCtx);
    auto readResponseCtxEncodeSize = sizeof(response1rawbuffer) - readResponseCtx.size();

    common::Buffer toReceive1((std::byte*)response1rawbuffer, readResponseCtxEncodeSize, false);

    EXPECT_CALL(mockISocket, sendto(
        Truly([&toReceive0](const common::Buffer& a) {return isMessageEqual(a, toReceive0);}),
        Truly([&from](const net::IpPort& a) {return isIpPortEqual(a, from);})
        , 0)).RetiresOnSaturation();

    EXPECT_CALL(mockISocket, sendto(
        Truly([&toReceive1](const common::Buffer& a) {return isMessageEqual(a, toReceive1);}),
        Truly([&from](const net::IpPort& a) {return isIpPortEqual(a, from);})
        , 0)).RetiresOnSaturation();

    {
        FlyDbMessage msg;
        msg.msg = WriteRequest{};
        msg.transactionId = 0;
        auto& request = std::get<WriteRequest>(msg.msg);
        request.paramIds.emplace_back(ParamIdData{1, {{42,0,0,0}}});
        request.paramIds.emplace_back(ParamIdData{2, {{43,0}}});
        request.paramIds.emplace_back(ParamIdData{3, {{44,0,0}}});
        request.paramIds.emplace_back(ParamIdData{4, {{45,0,0,0,0,0,0,0}}});

        cum::per_codec_ctx ctx(requestrawbuffer, sizeof(requestrawbuffer));
        encode_per(msg, ctx);
        auto msgSize = sizeof(requestrawbuffer) - ctx.size();

        dbServer.onReceive(common::Buffer(requestrawbuffer, msgSize, false), from);
    }

    {
        FlyDbMessage msg;
        msg.msg = ReadRequest{};
        msg.transactionId = 1;
        auto& request = std::get<ReadRequest>(msg.msg);
        request.paramIds.emplace_back(1);
        request.paramIds.emplace_back(2);
        request.paramIds.emplace_back(3);
        request.paramIds.emplace_back(4);

        cum::per_codec_ctx ctx(requestrawbuffer, sizeof(requestrawbuffer));
        encode_per(msg, ctx);
        auto msgSize = sizeof(requestrawbuffer) - ctx.size();

        dbServer.onReceive(common::Buffer(requestrawbuffer, msgSize, false), from);
    }
}