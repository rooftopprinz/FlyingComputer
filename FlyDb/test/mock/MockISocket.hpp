#ifndef __TEST_MOCK_MOCKISOCKET_HPP__
#define __TEST_MOCK_MOCKISOCKET_HPP__

#include <gmock/gmock.h>
#include <Udp.hpp>

struct MockISocket : public net::ISocket
{
    MOCK_METHOD1(bind, void(const net::IpPort& pAddr));
    MOCK_METHOD3(sendto, ssize_t(const common::Buffer& pData, const net::IpPort& pAddr, int pFlags));
    MOCK_METHOD3(recvfrom, ssize_t(common::Buffer& pData, net::IpPort& pAddr, int pFlags));
    MOCK_METHOD4(setsockopt, void(int pLevel, int pOptionName, const void *pOptionValue, socklen_t pOptionLen));
};

#endif // __TEST_MOCK_MOCKISOCKET_HPP__