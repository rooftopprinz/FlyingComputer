#ifndef __DBSERVER_HPP__
#define __DBSERVER_HPP__

#include <Udp.hpp>
#include <protocol.hpp>
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
            default:
                std::cout << "MessageId invalid.\n";
        }
    }

private:
    void onSetRequest(const common::Buffer& pMessage, const net::IpPort& pAddr)
    {
        flydb::SetRequest setRequest;
        std::memcpy(&setRequest, pMessage.data(), sizeof(setRequest));
        const std::byte *data = pMessage.data() + sizeof(setRequest);
        common::Buffer newData(new std::byte[setRequest.size], setRequest.size);
        std::memcpy(newData.data(), data, setRequest.size);
        mDatabase[setRequest.key] = std::move(newData);
        flydb::SetResponse response;
        response.header.msgType = flydb::MessageType::SetResponse;
        response.header.trId = setRequest.header.trId;
        mSocket.sendto(common::Buffer((std::byte*)&response, sizeof(response), false), pAddr);
    }

    std::map<uint8_t, common::Buffer> mDatabase;
    net::ISocket& mSocket;

};

#endif // __DBSERVER_HPP__