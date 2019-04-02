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
        flydb::Decoder<flydb::GetRequest> requestDecoder(pMessage.data(), pMessage.size());
        const auto& request = requestDecoder.get();

        std::byte responseraw[1024];

        flydb::Encoder<flydb::GetResponse> responseEncoder(responseraw, sizeof(responseraw));
        auto& response = *responseEncoder.initialize();

        response.header.trId = request.header.trId;

        for (flydb::Size i = 0; i < request.size; i++)
        {
            flydb::Key key;
            flydb::Size size;

            requestDecoder.getField(key);

            auto it = mDatabase.find(key);
            std::byte* data = nullptr;
            flydb::Size size = 0;
            if (mDatabase.end() != it)
            {
                data = it->second.data();
                size = it->second.size();
            }

            responseEncoder.addField(key, data, size);
        }

        response.size = responseEncoder.useSize();
        mSocket.sendto(common::Buffer(rawbuffer, sizeof(response) + response.size, false), pAddr);
    }

    void onSetRequest(const common::Buffer& pMessage, const net::IpPort& pAddr)
    {
        flydb::SetRequest request;
        std::memcpy(&request, pMessage.data(), sizeof(request));

        std::byte *requestptr = pMessage.data() + sizeof(request);
        std::byte *requestptrlimit = pMessage.data() + sizeof(request) + request.size;

        while (requestptr<requestptrlimit)
        {

            flydb::Key key;
            std::byte *data;
            flydb::Size size;

            flydb::getData(responseptr, key, data, size);

            auto it = mDatabase.find(key);
            if (mDatabase.end() != it && it->second.size()==size)
            {
                std::memcpy(it->second.data(), data, size);
            }
            else
            {
                std::byte* newdata = new std::byte[size];
                mDatabase.emplace(key, common::Buffer(newdata, size));
                std::memcpy(newdata, data, size);
            }
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