#ifndef __FLYDBCLIENT__
#define __FLYDBCLIENT__

#include <bfc/Udp.hpp>
#include <Logger.hpp>
#include <FlyDbInterface.hpp>

class FlyDbClient
{
public:
    enum Status {OK, TIMEOUT, BADRESP};
    FlyDbClient(const bfc::IpPort& pServerAddr, uint64_t pUsTimeout=1000)
        : mServerAddr(pServerAddr)
    {

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = pUsTimeout;

        mSock.setsockopt(SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
    }

    Status get(const ParamIdArray& pIds, ParamDataArray& pParamData)
    {
        uint8_t reqTrId = mTrId++;
        {
            FlyDbMessage msg;
            msg.msg = ReadRequest{};
            msg.transactionId = reqTrId;
            auto& request = std::get<ReadRequest>(msg.msg);
            for (auto i : pIds)
            {
                request.paramId.emplace_back(i);
            }
            cum::per_codec_ctx ctx(mBuffer, sizeof(mBuffer));
            encode_per(msg, ctx);
            auto msgSize = sizeof(mBuffer) - ctx.size();
            mSock.sendto(bfc::ConstBufferView(mBuffer, msgSize), mServerAddr);
        }

        while (true)
        {
            bfc::IpPort rcvaddr;
            bfc::BufferView rcvBuffer(mBuffer, sizeof(mBuffer));
            auto rc = mSock.recvfrom(rcvBuffer, rcvaddr);
            if (rc>0)
            {
                FlyDbMessage msg {};
                cum::per_codec_ctx ctx(mBuffer, rc);
                decode_per(msg, ctx);

                if (reqTrId != msg.transactionId)
                {
                    // late response are discarded
                    continue;
                }
                else if (1 != msg.msg.index()) // TODO: get index by type getter
                {
                    return BADRESP;
                }

                auto& readReponse = std::get<ReadResponse>(msg.msg);
                pParamData = std::move(readReponse.paramData);

                return OK;
            }
            return TIMEOUT;
        }
    }

    Status set(ParamIdDataArray&& pParamIdData)
    {
        auto reqTrId = mTrId++;
        {
            FlyDbMessage msg;
            msg.msg = WriteRequest{};
            msg.transactionId = reqTrId;
            auto& request = std::get<WriteRequest>(msg.msg);
            request.paramIdData = std::move(pParamIdData);

            cum::per_codec_ctx ctx(mBuffer, sizeof(mBuffer));
            encode_per(msg, ctx);
            auto msgSize = sizeof(mBuffer) - ctx.size();
            mSock.sendto(bfc::ConstBufferView(mBuffer, msgSize), mServerAddr);
        }
        while (true)
        {
            bfc::IpPort rcvaddr;
            bfc::BufferView rcvBuffer(mBuffer, sizeof(mBuffer));
            auto rc = mSock.recvfrom(rcvBuffer, rcvaddr);
            if (rc>0)
            {
                FlyDbMessage msg {};
                cum::per_codec_ctx ctx(mBuffer, rc);
                decode_per(msg, ctx);

                if (msg.transactionId != reqTrId)
                {
                    // late response are discarded
                    continue;
                }
                else if (4 != msg.msg.index()) // TODO: get index by type getter
                {
                    return BADRESP;
                }
                
                return OK;
            }
            return TIMEOUT;
        }
    }

private:
    uint8_t mTrId = 0;
    bfc::UdpSocket mSock = {};
    std::byte  mBuffer[1024];
    bfc::IpPort mServerAddr;
};

#endif // __FLYDBCLIENT__