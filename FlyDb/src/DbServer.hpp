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

    void onReceive(const common::Buffer& pMessage, const net::IpPort& pAddr)
    {
        FlyDbMessage message;
        cum::per_codec_ctx ctx(pMessage.data(), pMessage.size());
        decode_per(message, ctx);
        std::visit([this, &pAddr](auto&& pArg){this->handle(arg, pAddr)});
    }

private:
    void handle(const ReadRequest& pMsg, const net::IpPort& pAddr)
    {
        ReadResponse response;
        for (auto id : pMsg.paramIds)
        {
            response.paramData.emplace_back();
            auto& respItem = response.paramData.back();
            auto found = mDatabase.find(id);
            if (found != mDatabase.end())
            {
                respItem = *found;
            }
        }

        encodeAndSend(response, pAddr);
    }

    void handle(const WriteRequest& pMsg, const net::IpPort& pAddr)
    {
        WriteResponse response;
        for (auto paramIdData : pMsg.paramIds)
        {
            auto& item = mDatabase[paramIdData.id];
            item = std::move(paramIdData.data);
        }
        encodeAndSend(response, pAddr);
    }

    void handle(const WriteIndication& pMsg, const net::IpPort& pAddr)
    {
        for (auto paramIdData : pMsg.paramIds)
        {
            auto& item = mDatabase[paramIdData.id];
            item = std::move(paramIdData.data);
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