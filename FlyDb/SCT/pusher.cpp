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
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    sock.setsockopt(SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));


    std::byte requestrawbuffer[1024];
    std::byte responserawbuffer[1024];
    common::Buffer responsebuffer(responserawbuffer, sizeof(responserawbuffer), false);

    int i = 0;
    uint8_t trId = 0;
    for (;;i++)
    {

        // SET REQUEST
        {
            Logless("SET REQUEST \\");
            auto trId0 = trId++;
            {
                FlyDbMessage msg;
                msg.msg = WriteRequest{};
                msg.transactionId = trId0;
                auto& request = std::get<WriteRequest>(msg.msg);
                for (uint8_t j=0u; j<count; j++)
                {
                    std::vector<uint8_t> buff;
                    buff.assign((uint8_t*)&i, (uint8_t*)&i+sizeof(i));
                    request.paramIds.emplace_back(ParamIdData{1, {{}}});
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
                flydb::Decoder<flydb::SetResponse> setResponseDecoder(responserawbuffer, sizeof(responserawbuffer));
                auto response = setResponseDecoder.get();
                if (response.trId != trId0)
                {
                    Logless("Received trId expected:_ actual:_", (unsigned)trId0, (unsigned)response.trId);
                }
                else if (response.msgType != flydb::MessageType::SetResponse)
                {
                    Logless("Received response unexpected:_", (unsigned)response.msgType);
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
            Logless("GET REQUEST \\");
            flydb::Encoder<flydb::GetRequest> getRequestEncoder(requestrawbuffer, sizeof(requestrawbuffer));
            getRequestEncoder.get().trId = trId;
            auto trId0 = trId++;

            for (flydb::Key j=0; j<count; j++)
            {
                getRequestEncoder.addField(flydb::Key(base+j));
                Logless("getting [_]", base+j);
            }
            Logless("GET REQUEST /");

            sock.sendto(common::Buffer(requestrawbuffer, getRequestEncoder.size(), false), serveraddr);

            auto rc = sock.recvfrom(responsebuffer, rcvaddr);

            Logless("GET RESPONSE \\");
            Logless("message =_", BufferLog(rc, responserawbuffer));
            if (rc>0)
            {
                flydb::Decoder<flydb::GetResponse> getResponseDecoder(responserawbuffer, rc);
                auto response = getResponseDecoder.get();
                int j = 0;
                while (true)
                {
                    flydb::Key key;
                    int value;
                    if (!getResponseDecoder.getField(key, value))
                    {
                        break;
                    }
                    if (key!=base+j || value!=i)
                    {
                        Logless("Get [_]=_ is unexpected!", key, value);
                        break;
                    }
                    j++;
                }

                if (j!=count)
                {
                    Logless("Get items unexpected! expected=_ acutal=_", count, j);
                }

                if (response.trId != trId0)
                {
                    Logless("Received trId expected:_ actual:_", (unsigned)trId0, (unsigned)response.trId);
                }
                if (response.msgType != flydb::MessageType::GetResponse)
                {
                    Logless("Received response unexpected:_", (unsigned)response.msgType);
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
