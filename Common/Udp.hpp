#ifndef __UDP_HPP__
#define __UDP_HPP__

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <memory>
#include <Buffer.hpp>
namespace net
{

struct IpPort
{
    IpPort() = default;
    IpPort (uint32_t pAddr, uint16_t pPort)
        : addr(pAddr)
        , port(pPort)
    {}

    uint32_t addr;
    uint16_t port;
};

struct ISocket
{
    virtual void bind(const IpPort& pAddr) = 0;
    virtual ssize_t sendto(const common::Buffer& pData, const IpPort& pAddr, int pFlags=0) = 0;
    virtual ssize_t recvfrom(common::Buffer& pData, IpPort& pAddr, int pFlags=0) = 0;
    virtual void setsockopt(int pLevel, int pOptionName, const void *pOptionValue, socklen_t pOptionLen) = 0;
};

struct IUdpFactory
{
    virtual std::unique_ptr<ISocket> create() = 0;
};

class UdpSocket : public ISocket
{
public:
    UdpSocket()
        : mSockFd(socket(AF_INET, SOCK_DGRAM, 0))
    {
        if (mSockFd  < 0)
        {
            std::string err = std::string("Socket creation failed! Error: ") + strerror(errno);
            throw std::runtime_error(err);
        }
    }

    ~UdpSocket()
    {
        if (mSockFd>=0)
        {
            close(mSockFd);
        }
    }

    void bind(const IpPort& pAddr)
    {
        sockaddr_in myaddr;
        memset((char *)&myaddr, 0, sizeof(myaddr));
        myaddr.sin_family = AF_INET;
        myaddr.sin_addr.s_addr = htonl(pAddr.addr);
        myaddr.sin_port = htons(pAddr.port);

        int rv = ::bind(mSockFd, (sockaddr *)&myaddr, sizeof(myaddr));
        if (rv < 0)
        {
            std::string err = std::string("Bind failed: ") + std::to_string(rv) + " Error: " + strerror(errno);
            throw std::runtime_error(err);
        }
    }

    ssize_t sendto(const common::Buffer& pData, const IpPort& pAddr, int pFlags=0)
    {
        sockaddr_in to;
        to.sin_family = AF_INET;
        to.sin_addr.s_addr = htonl(pAddr.addr);
        to.sin_port = htons(pAddr.port);
        return ::sendto(mSockFd, pData.data(), pData.size(), pFlags, (sockaddr*)&to, sizeof(to));
    }

    ssize_t recvfrom(common::Buffer& pData, IpPort& pAddr, int pFlags=0)
    {
        sockaddr_in framddr;
        socklen_t framddrSz = sizeof(framddr);
        auto sz = ::recvfrom(mSockFd, pData.data(), pData.size(), pFlags, (sockaddr*)&framddr, &framddrSz);
        pAddr.addr = ntohl(framddr.sin_addr.s_addr);
        pAddr.port = ntohs(framddr.sin_port);
        return sz;
    }

    void setsockopt(int pLevel, int pOptionName, const void *pOptionValue, socklen_t pOptionLen)
    {
        int rv = ::setsockopt(mSockFd, pLevel, pOptionName, pOptionValue, pOptionLen);
        if (rv < 0)
        {
            std::string err = std::string("Setsockopt failed: ") + std::to_string(rv) + " Error: " + strerror(errno);
            throw std::runtime_error(err);
        }
    }

private:
    int mSockFd;
};

class UdpFactory : public IUdpFactory
{
public:
    std::unique_ptr<ISocket> create()
    {
        return std::make_unique<UdpSocket>();
    }
};

inline IpPort toIpPort(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port)
{
    return IpPort(uint32_t((a<<24)|(b<<16)|(c<<8)|d), port);
}

}

#endif // __UDP_HPP__