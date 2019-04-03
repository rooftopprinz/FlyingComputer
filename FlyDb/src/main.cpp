#include <Udp.hpp>
#include <DbServer.hpp>

int main()
{
    net::UdpSocket sock;
    net::IpPort addr(0, 9000);
    sock.bind(addr);
    DbServer server(sock);
    std::byte rawbuffer[1024];
    common::Buffer recvbuffer(rawbuffer, sizeof(rawbuffer), false);

    Logger::getInstance().logful();

    while (true)
    {
        auto rc = sock.recvfrom(recvbuffer, addr);
        if (rc>0)
        {
            server.onReceive(common::Buffer(recvbuffer.data(), rc, false), addr);
        }
        else
        {
            Logless("receive: rc=_ error=_", rc, (const char*)strerror(errno));
        }
    }
}