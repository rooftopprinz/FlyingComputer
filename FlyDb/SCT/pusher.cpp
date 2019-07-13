#include <Udp.hpp>
#include <Logger.hpp>
#include <FlyDbInterface.hpp>
#include <protocol.hpp>

int main(int argc, const char *argv[])
{

    net::UdpSocket sock;
    net::IpPort serveraddr = net::toIpPort(127,0,0,1,9000);
    net::IpPort rcvaddr;

    int base  = std::atoi(argv[1]);
    int count = std::atoi(argv[2]);

    Logger::getInstance().logful();

    Logless("base=_ count=_", base, count);

    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 10000;

    sock.setsockopt(SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));


    std::byte requestrawbuffer[1024];
    std::byte responserawbuffer[1024];
    common::Buffer responsebuffer(responserawbuffer, sizeof(responserawbuffer), false);

    int i = 0;
    uint8_t trId = 0;
    for (;;i++,trId++)
    {

        // SET REQUEST
        {
            auto trId0 = trId++;
            Logless("SET REQUEST trid=_ \\", (unsigned)trId0);
            {
                FlyDbMessage msg;
                msg.msg = WriteRequest{};
                msg.transactionId = trId0;
                auto& request = std::get<WriteRequest>(msg.msg);
                for (uint8_t j=0u; j<count; j++)
                {
                    request.paramIdData.emplace_back(ParamIdData{uint8_t(base+j), i});
                    Logless("setting [_]=_", base+j, i);
                }
                cum::per_codec_ctx ctx(requestrawbuffer, sizeof(requestrawbuffer));
                encode_per(msg, ctx);
                auto msgSize = sizeof(requestrawbuffer) - ctx.size();
                sock.sendto(common::Buffer(requestrawbuffer, msgSize, false), serveraddr);
            }
            Logless("SET REQUEST /");

            auto rc = sock.recvfrom(responsebuffer, rcvaddr);

            Logless("SET RESPONSE \\");
            if (rc>0)
            {
                FlyDbMessage response {};
                cum::per_codec_ctx ctx(responserawbuffer, rc);
                decode_per(response, ctx);

                if (response.transactionId != trId0)
                {
                    Logless("Received trId expected:_ actual:_", (unsigned)trId0, (unsigned)response.transactionId);
                }
                else if (4 != response.msg.index())
                {
                    Logless("Received response unexpected:_", response.msg.index());
                }
            }
            else
            {
                Logless("Receive Error! rc=_ error=_", rc, (const char*)strerror(errno));
            }
            Logless("SET RESPONSE /");
        }

        // GET
        {
            auto trId0 = trId++;
            Logless("GET REQUEST \\");
            {
                FlyDbMessage msg;
                msg.msg = ReadRequest{};
                msg.transactionId = trId0;
                auto& request = std::get<ReadRequest>(msg.msg);
                for (uint8_t j=0u; j<count; j++)
                {
                    request.paramId.emplace_back(uint8_t(base+j));
                    Logless("getting [_]", base+j);
                }
                cum::per_codec_ctx ctx(requestrawbuffer, sizeof(requestrawbuffer));
                encode_per(msg, ctx);
                auto msgSize = sizeof(requestrawbuffer) - ctx.size();
                sock.sendto(common::Buffer(requestrawbuffer, msgSize, false), serveraddr);
            }
            Logless("GET REQUEST /");

            auto rc = sock.recvfrom(responsebuffer, rcvaddr);

            Logless("GET RESPONSE \\");
            if (rc>0)
            {
                FlyDbMessage msg {};
                cum::per_codec_ctx ctx(responserawbuffer, rc);
                decode_per(msg, ctx);

                if (msg.transactionId != trId0)
                {
                    Logless("Received trId expected:_ actual:_", (unsigned)trId0, (unsigned)msg.transactionId);
                }
                else if (1 != msg.msg.index())
                {
                    Logless("Received response unexpected:_", msg.msg.index());
                }
                else
                {
                    auto& response = std::get<ReadResponse>(msg.msg);
                    int i = 0;
                    for (auto& e : response.paramData)
                    {
                        std::visit([i](auto&& pData){
                            using DATA_TYPE = std::decay_t<decltype(pData)>;
                            if constexpr (std::is_same_v<DATA_TYPE, int>)
                            {
                                Logless("get [_] = _", i, pData);
                            }
                            else
                            {
                                Logless("get [_] is unuset", i);

                            }}, e);
                        i++;
                    }
                }
            }
            else
            {
                Logless("Receive Error! rc=_ error=_", rc, (const char*)strerror(errno));
            }
            Logless("GET RESPONSE /");
        }
    }
}
