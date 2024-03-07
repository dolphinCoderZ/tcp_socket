#include "TcpSocket.h"
#include <iostream>
using namespace std;

#define BACKLOG 5

class TcpServer
{
public:
    TcpServer();
    ~TcpServer();

    int setListen(unsigned short port);

    TcpSocket *acceptConn(struct sockaddr_in *addr);

private:
    int m_fd;
};

TcpServer::TcpServer()
{
    this->m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->m_fd < 0)
    {
        cout << "server fd err" << endl;
        exit(2);
    }
}

TcpServer::~TcpServer()
{
    if (this->m_fd >= 0)
    {
        close(this->m_fd);
    }
}

int TcpServer::setListen(unsigned short port)
{
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(this->m_fd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (ret < 0)
    {
        cout << "bind err" << endl;
        return -1;
    }

    cout << "ip: " << inet_ntoa(saddr.sin_addr) << ", port: " << port << endl;

    ret = listen(this->m_fd, BACKLOG);
    if (ret < 0)
    {
        cout << "listen err" << endl;
        return -1;
    }
    cout << "listening" << endl;
    return ret;
}

TcpSocket *TcpServer::acceptConn(sockaddr_in *addr = nullptr)
{
    if (addr == nullptr)
    {
        return nullptr;
    }

    socklen_t addrLen = sizeof(struct sockaddr_in);
    int recvFd = accept(this->m_fd, (struct sockaddr *)addr, &addrLen);

    if (recvFd < 0)
    {
        cout << "accept err" << endl;
        return nullptr;
    }
    cout << "accept client" << endl;
    return new TcpSocket(recvFd);
}
