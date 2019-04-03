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
            case flydb::MessageType::SetIndication:
                onSetIndication(pMessage, pAddr);
                break;
            case flydb::MessageType::GetRequest:
                onGetRequest(pMessage, pAddr);
                break;
            default:
                Logless("ERR DbServer::onReceive - MessageId _ invalid.", (unsigned)header.msgType);
        }
    }

private:
    void onGetRequest(const common::Buffer& pMessage, const net::IpPort& pAddr)
    {
        std::byte responseraw[1024];
        flydb::Decoder<flydb::GetRequest>  requestDecoder(pMessage.data(), pMessage.size());
        flydb::Encoder<flydb::GetResponse> responseEncoder(responseraw, sizeof(responseraw));
        Logless("DBG DbServer::onGetRequest - message =_", BufferLog(pMessage.size(), (void*)pMessage.data()));

        auto request = requestDecoder.get();
        auto& response = responseEncoder.get();
        response.trId = request.trId;

        while (true)
        {
            flydb::Key key;

            if (!requestDecoder.getField(key))
            {
                break;
            }

            auto it = mDatabase.find(key);

            std::byte* data = nullptr;
            flydb::Size size = 0;

            if (mDatabase.end() != it)
            {
                data = it->second.data();
                size = it->second.size();
            }

            Logless("DBG DbServer::onGetRequest - Get [_] sz:_", (unsigned)key, (unsigned)size);
            if (!responseEncoder.addField(key, data, size))
            {
                Logless("ERR DbServer::onGetRequest - unable to encode reponse!");
                return;
            }
        }

        mSocket.sendto(common::Buffer(responseraw, responseEncoder.size(), false), pAddr);
    }

    void onSetRequest(const common::Buffer& pMessage, const net::IpPort& pAddr)
    {
        std::byte responseraw[1024];
        flydb::Decoder<flydb::SetRequest>  requestDecoder(pMessage.data(), pMessage.size());
        flydb::Encoder<flydb::SetResponse> responseEncoder(responseraw, sizeof(responseraw));

        auto request = requestDecoder.get();
        auto& response = responseEncoder.get();
        response.trId = request.trId;

        while (true)
        {
            flydb::Key key;
            const std::byte *data;
            flydb::Size size;

            if (!requestDecoder.getField(key, data, size))
            {
                break;
            }

            auto it = mDatabase.find(key);
            if (mDatabase.end() != it && it->second.size()==size)
            {
                std::memcpy(it->second.data(), data, size);
            }
            else
            {
                std::byte* newdata = new std::byte[size];
                std::memcpy(newdata, data, size);
                mDatabase.emplace(key, common::Buffer(newdata, size));
            }
        }

        mSocket.sendto(common::Buffer(responseraw, responseEncoder.size(), false), pAddr);
    }

    void onSetIndication(const common::Buffer& pMessage, const net::IpPort& pAddr)
    {
        flydb::Decoder<flydb::SetRequest> requestDecoder(pMessage.data(), pMessage.size());

        while (true)
        {
            flydb::Key key;
            const std::byte *data;
            flydb::Size size;

            if (!requestDecoder.getField(key, data, size))
            {
                break;
            }

            auto it = mDatabase.find(key);
            if (mDatabase.end() != it && it->second.size()==size)
            {
                std::memcpy(it->second.data(), data, size);
            }
            else
            {
                std::byte* newdata = new std::byte[size];
                std::memcpy(newdata, data, size);
                mDatabase.emplace(key, common::Buffer(newdata, size));
            }
        }
    }

    std::map<uint8_t, common::Buffer> mDatabase;
    net::ISocket& mSocket;
};

#endif // __DBSERVER_HPP__