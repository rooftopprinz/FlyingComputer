#ifndef __DBSERVER_HPP__
#define __DBSERVER_HPP__

#include <Udp.hpp>
#include <protocol.hpp>
#include <Logger.hpp>
#include <map>

class DbServer
{
public:
    DbServer(net::ISocket& pSocket)
        : mSocket(pSocket)
    {}

    void onReceive(const common::Buffer& pMessage, const net::IpPort& pAddr)
    {
        flydb::Header header;
        std::memcpy(&header, pMessage.data(), sizeof(header));
        switch (header.msgType)
        {
            case flydb::MessageType::SetRequest:
                onSetRequest(pMessage, pAddr);
                break;
            case flydb::MessageType::GetRequest:
                onGetRequest(pMessage, pAddr);
            default:
                Logless("MessageId _ invalid.", (unsigned)header.msgType);
        }
    }

private:
    void onGetRequest(const common::Buffer& pMessage, const net::IpPort& pAddr)
    {
        flydb::GetRequest request;
        std::memcpy(&request, pMessage.data(), sizeof(request));

        flydb::GetResponse response;
        response.header.msgType = flydb::MessageType::GetResponse;
        response.header.trId = request.header.trId;
        response.size = 0;
        std::byte *data = nullptr;

        auto it = mDatabase.find(request.key);
        if (mDatabase.end() != it)
        {
            data = it->second.data();
            response.size = it->second.size();
        }

        std::byte rawbuffer[128];
        std::memcpy(rawbuffer, &response, sizeof(response));
        std::memcpy(rawbuffer+sizeof(response), data, response.size);
        mSocket.sendto(common::Buffer(rawbuffer, sizeof(response) + response.size, false), pAddr);
    }

    void onSetRequest(const common::Buffer& pMessage, const net::IpPort& pAddr)
    {
        flydb::SetRequest request;
        std::memcpy(&request, pMessage.data(), sizeof(request));
        const std::byte *data = pMessage.data() + sizeof(request);

        auto it = mDatabase.find(request.key);
        if (mDatabase.end() != it && it->second.size()==request.size)
        {
            std::memcpy(it->second.data(), data, request.size);
        }
        else
        {
            mDatabase.emplace(request.key, common::Buffer(new std::byte[request.size], request.size));
        }

        flydb::SetResponse response;
        response.header.msgType = flydb::MessageType::SetResponse;
        response.header.trId = request.header.trId;
        mSocket.sendto(common::Buffer((std::byte*)&response, sizeof(response), false), pAddr);
    }

    std::map<uint8_t, common::Buffer> mDatabase;
    net::ISocket& mSocket;
};

#endif // __DBSERVER_HPP__