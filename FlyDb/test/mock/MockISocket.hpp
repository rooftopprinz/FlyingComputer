#ifndef __TEST_MOCK_MOCKISOCKET_HPP__
#define __TEST_MOCK_MOCKISOCKET_HPP__

#include <gmock/gmock.h>
#include <bfc/Udp.hpp>

struct MockISocket : public bfc::ISocket
{
    MOCK_METHOD1(bind, void(const bfc::IpPort& pAddr));
    MOCK_METHOD3(sendto, ssize_t(const bfc::ConstBufferView& pData, const bfc::IpPort& pAddr, int pFlags));
    MOCK_METHOD3(recvfrom, ssize_t(bfc::BufferView& pData, bfc::IpPort& pAddr, int pFlags));
    MOCK_METHOD4(setsockopt, void(int pLevel, int pOptionName, const void *pOptionValue, socklen_t pOptionLen));
};

#endif // __TEST_MOCK_MOCKISOCKET_HPP__