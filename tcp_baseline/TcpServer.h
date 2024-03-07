#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define BACKLOG 5

class TcpServer
{
public:
    TcpServer() : m_fd(-1){};
    TcpServer(int port);
    ~TcpServer();

    void initSever();
    void startServer();
    void service(int fd, string clientIp, int clientPort);

private:
    int m_fd;
    int m_Port;
};

TcpServer::TcpServer(int port)
{
    // �׽��־����ļ�����Э���壬�׽������ͣ�Э�����ͣ�
    this->m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->m_fd < 0)
    {
        perror("fd");
        exit(2);
    }
    this->m_Port = port;
}

TcpServer::~TcpServer()
{
    if (this->m_fd >= 0)
    {
        close(this->m_fd);
    }
}

void TcpServer::initSever()
{
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));

    // ��������Э��
    saddr.sin_family = AF_INET;
    // ����������ת��Ϊ��������
    saddr.sin_port = htons(this->m_Port);
    saddr.sin_addr.s_addr = INADDR_ANY;

    // ���׽����������
    if (bind(this->m_fd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    {
        perror("bind");
        exit(3);
    }

    // ���÷�����Ϊ����״̬
    if (listen(this->m_fd, BACKLOG) < 0)
    {
        perror("listen");
        exit(4);
    }
}

void TcpServer::startServer()
{
    for (;;)
    {
        struct sockaddr_in peer;
        memset(&peer, 0, sizeof(peer));
        socklen_t addrLen = sizeof(peer);

        // �����ͻ��ˣ�������
        int recvSocket = accept(this->m_fd, (struct sockaddr *)&peer, &addrLen);
        if (recvSocket < 0)
        {
            perror("continue next");
            continue;
        }
        // ����������->��������->���ʮ����IP
        string clientIp = inet_ntoa(peer.sin_addr);
        int clientPort = ntohs(peer.sin_port);
        cout << "new link->" << recvSocket << " [" << clientIp << " ]:" << clientPort << endl;

        this->service(recvSocket, clientIp, clientPort);
    }
}

void TcpServer::service(int fd, string clientIp, int clientPort)
{
    char buffer[1024];
    while (true)
    {
        ssize_t size = read(fd, buffer, sizeof(buffer) - 1);
        if (size > 0)
        {
            buffer[size] = '\0';
            cout << "new link-> " << fd << " [" << clientIp << " ]:" << clientPort << endl;

            write(fd, buffer, size);
        }
        else if (size == 0)
        {
            cout << "client close" << endl;
        }
        else
        {
            cout << "read err" << endl;
        }
    }
    close(fd);
    cout << "server done" << endl;
}
