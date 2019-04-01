#include <Udp.hpp>
#include <FlyDbClient.hpp>
#include <Logger.hpp>

int main(int argc, const char *argv[])
{

    net::UdpSocket sock;
    net::IpPort serveraddr = net::toIpPort(127,0,0,1,9000);
    net::IpPort rcvaddr;

    std::byte rawbuffer[1024];
    common::Buffer buffer(rawbuffer, sizeof(rawbuffer), false);

    int base  = std::atoi(argv[1]);
    int count = std::atoi(argv[2]);

    timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    sock.setsockopt(SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv));
    Logger::getInstance().logful();

    int i = 0;
    for (;;i++)
    {
        for (int j=0; j<count; j++)
        {
            // SET
            auto rc = createSetRequest(i&0xFF, base+j, i, buffer);
            sock.sendto(common::Buffer(buffer.data(), rc, false), serveraddr);
            Logless("setting [_]=_", base+j, i);
            rc = sock.recvfrom(buffer, rcvaddr);
            if (rc>0)
            {
                auto header = getHeader(buffer);
                if(header.trId != (i&0xFF))
                {
                    Logless("Received trId expected:_ actual:_", (i&0xFF), (unsigned)header.trId);
                }
                else if (header.msgType != flydb::MessageType::SetResponse)
                {
                    Logless("Received response unexpected:_", (unsigned)header.msgType);
                }
            }
            else
            {
                Logless("Receive Error! rc=_ error=_", rc, (const char*)strerror(errno));
            }

            // GET
            flydb::GetRequest getrequest;
            getrequest.header.msgType = flydb::MessageType::GetRequest;
            getrequest.header.trId = i&0xFF;
            getrequest.key = base+j;
            sock.sendto(common::Buffer((std::byte*)&getrequest, sizeof(getrequest), false), serveraddr);
            Logless("getting [_]", base+j, i);
            rc = sock.recvfrom(buffer, rcvaddr);
            if (rc>0)
            {
                auto header = getHeader(buffer);
                if(header.trId != (i&0xFF))
                {
                    Logless("Received trId expected:_ actual:_", (i&0xFF), (unsigned)header.trId);
                }
                else if (header.msgType != flydb::MessageType::GetResponse)
                {
                    Logless("Received response unexpected:_", (unsigned)header.msgType);
                }
                else
                {
                    auto v = getDataFromGetResponse<int>(buffer);
                    Logless("GetResponse [_]=_", base+j, v);
                }
            }
            else
            {
                Logless("Receive Error! rc=_ error=_", rc, (const char*)strerror(errno));
            }
        }
    }
}
