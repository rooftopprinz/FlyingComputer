#ifndef __DBSERVER_HPP__
#define __DBSERVER_HPP__

#include <map>
#include <bfc/Udp.hpp>
#include <Logger.hpp>
#include <protocol.hpp>
#include <FlyDbInterface.hpp>

class DbServer
{
public:
    DbServer(bfc::ISocket& pSocket)
        : mSocket(pSocket)
    {}

    void onReceive(bfc::BufferView&& pMessageBuffer, const bfc::IpPort& pAddr)
    {
        auto trace = Trace("onReceive()");
        Logless("Received from _:_ data=_", pAddr.addr, pAddr.port, BufferLog(pMessageBuffer.size(), pMessageBuffer.data()));
        FlyDbMessage message {};
        cum::per_codec_ctx ctx(pMessageBuffer.data(), pMessageBuffer.size());
        try
        {
            decode_per(message, ctx);
        }
        catch(std::exception& e)
        {
            Logless("Receive decode failed! what=_", e.what());
        }

        std::string receivedMsgStr;
        str("FlyDbMessage", message, receivedMsgStr, true);
        Logless("Received message: _", receivedMsgStr.c_str());
        std::visit([this, &pAddr, &message](auto&& pArg){this->handle(message, pArg, pAddr);}, message.msg);
    }

private:
    class Trace
    {
    public:
        Trace(const char* pName)
            : mName(pName)
        {
            Logless("TRACE _ \\", mName);
        }
        ~Trace()
        {
            Logless("TRACE _ /", mName);
        }
    private:
        const char* mName = nullptr;
    };

    template <typename S, typename T>
    void handle(S&&, T&&, const bfc::IpPort&)
    {
        auto trace = Trace("handle(): generic");
    }

    void handle(FlyDbMessage& pRootMsg, ReadRequest& pMsg, const bfc::IpPort& pAddr)
    {
        auto trace = Trace("handle(): ReadRequest");

        FlyDbMessage responseRoot;
        responseRoot.msg = ReadResponse{};
        auto& response = std::get<ReadResponse>(responseRoot.msg);
        responseRoot.transactionId = pRootMsg.transactionId;

        for (auto id : pMsg.paramId)
        {
            response.paramData.emplace_back();
            auto& respItem = response.paramData.back();
            auto found = mDatabase.find(id);
            if (found != mDatabase.end())
            {
                respItem = found->second;
            }
        }

        encodeAndSend(responseRoot, pAddr);
    }

    void handle(FlyDbMessage& pRootMsg, WriteRequest& pMsg, const bfc::IpPort& pAddr)
    {
        auto trace = Trace("handle(): WriteRequest");

        FlyDbMessage responseRoot;
        responseRoot.msg = WriteResponse{};
        auto& response = std::get<WriteResponse>(responseRoot.msg);
        responseRoot.transactionId = pRootMsg.transactionId;
        response.spare = 0;
        for (auto paramIdData : pMsg.paramIdData)
        {
            auto& item = mDatabase[paramIdData.id];
            item = std::move(paramIdData.data);
        }
        encodeAndSend(responseRoot, pAddr);
    }

    void handle(FlyDbMessage&, WriteIndication& pMsg, const bfc::IpPort& pAddr)
    {
        auto trace = Trace("handle(): WriteIndication");
        for (auto paramIdData : pMsg.paramIdData)
        {
            auto& item = mDatabase[paramIdData.id];
            item = std::move(paramIdData.data);
        }
    }

    template <typename T>
    void encodeAndSend(const T& pResponse, const bfc::IpPort& pAddr)
    {
        auto trace = Trace("encodeAndSend()");

        std::string msgRespStr;
        str("FlyDbMessage", pResponse, msgRespStr, true);
        Logless("to encode message: _", msgRespStr.c_str());

        std::byte sendBuffer[512];
        cum::per_codec_ctx ctx(sendBuffer, sizeof(sendBuffer));
        encode_per(pResponse, ctx);
        auto encodeSize = sizeof(sendBuffer) - ctx.size();
        Logless("Sending to _:_ data=_", pAddr.addr, pAddr.port, BufferLog(encodeSize, sendBuffer));
        mSocket.sendto(bfc::Buffer(sendBuffer, encodeSize, [](std::byte*){}), pAddr);
    }

    std::map<uint8_t, ParamData> mDatabase;
    bfc::ISocket& mSocket;
};

#endif // __DBSERVER_HPP__