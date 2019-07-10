#ifndef __DBSERVER_HPP__
#define __DBSERVER_HPP__

#include <map>
#include <Udp.hpp>
#include <Logger.hpp>
#include <protocol.hpp>
#include <FlyDbInterface.hpp>


using DbData = std::vector<uint8_t>;
class DbServer
{
public:
    DbServer(net::ISocket& pSocket)
        : mSocket(pSocket)
    {}

    void onReceive(common::Buffer&& pMessage, const net::IpPort& pAddr)
    {
        FlyDbMessage message;
        cum::per_codec_ctx ctx(pMessage.data(), pMessage.size());
        decode_per(message, ctx);
        std::visit([this, &pAddr, &message](auto&& pArg){this->handle(message, pArg, pAddr);}, message.msg);
    }

private:
    template <typename S, typename T>
    void handle(S&&, T&&, const net::IpPort&)
    {}

    void handle(FlyDbMessage& pRootMsg, ReadRequest& pMsg, const net::IpPort& pAddr)
    {
        FlyDbMessage responseRoot;
        responseRoot.msg = ReadResponse{};
        auto& response = std::get<ReadResponse>(responseRoot.msg);
        responseRoot.transactionId = pRootMsg.transactionId;

        for (auto id : pMsg.paramIds)
        {
            response.paramData.emplace_back();
            auto& respItem = response.paramData.back().value;
            auto found = mDatabase.find(id);
            if (found != mDatabase.end())
            {
                respItem = found->second;
            }
        }

        encodeAndSend(responseRoot, pAddr);
    }

    void handle(FlyDbMessage& pRootMsg, WriteRequest& pMsg, const net::IpPort& pAddr)
    {
        FlyDbMessage responseRoot;
        responseRoot.msg = WriteResponse{};
        auto& response = std::get<WriteResponse>(responseRoot.msg);
        responseRoot.transactionId = pRootMsg.transactionId;
        response.spare = 0;
        for (auto paramIdData : pMsg.paramIds)
        {
            auto& item = mDatabase[paramIdData.id];
            item = std::move(paramIdData.data.value);
        }
        encodeAndSend(responseRoot, pAddr);
    }

    void handle(FlyDbMessage&, WriteIndication& pMsg, const net::IpPort& pAddr)
    {
        for (auto paramIdData : pMsg.paramIds)
        {
            auto& item = mDatabase[paramIdData.id];
            item = std::move(paramIdData.data.value);
        }
    }

    template <typename T>
    void encodeAndSend(const T& response, const net::IpPort& pAddr)
    {
        std::byte sendBuffer[512];
        cum::per_codec_ctx ctx(sendBuffer, sizeof(sendBuffer));
        encode_per(response, ctx);
        auto encodeSize = sizeof(sendBuffer) - ctx.size();
        mSocket.sendto(common::Buffer(sendBuffer, encodeSize, false), pAddr);
    }

    std::map<uint8_t, DbData> mDatabase;
    net::ISocket& mSocket;
};

#endif // __DBSERVER_HPP__