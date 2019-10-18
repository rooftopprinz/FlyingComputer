#include <bfc/Udp.hpp>
#include <DbServer.hpp>

int main()
{
    bfc::UdpSocket sock;
    bfc::IpPort addr(0, 9000);
    sock.bind(addr);
    DbServer server(sock);
    std::byte rawbuffer[1024];
    bfc::BufferView recvbuffer(rawbuffer, sizeof(rawbuffer));
    Logger::getInstance().logful();

    while (true)
    {
        auto rc = sock.recvfrom(recvbuffer, addr);
        if (rc>0)
        {
            server.onReceive(bfc::BufferView(rawbuffer, rc), addr);
        }
        else
        {
            Logless("receive: rc=_ error=_", rc, (const char*)strerror(errno));
        }
    }
}