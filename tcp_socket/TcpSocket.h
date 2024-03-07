#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
#include <arpa/inet.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

class TcpSocket
{
public:
    TcpSocket();
    TcpSocket(int fd);
    ~TcpSocket();
    int connectToHost(string ip, unsigned short port);
    int sendMsg(string msg);
    string recvMsg();

private:
    int readN(char *buffer, int size);
    int writeN(const char *msg, int size);

private:
    int m_fd;
};

TcpSocket::TcpSocket()
{
    this->m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->m_fd < 0)
    {
        cout << "create fd err" << endl;
        exit(1);
    }
}

TcpSocket::TcpSocket(int fd)
{
    this->m_fd = fd;
}

TcpSocket::~TcpSocket()
{
    if (this->m_fd >= 0)
    {
        close(this->m_fd);
    }
}

int TcpSocket::connectToHost(string ip, unsigned short port)
{
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;
    // 将IP地址转换为网络序列
    inet_pton(AF_INET, ip.data(), &saddr.sin_addr.s_addr);

    if (connect(this->m_fd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    {
        cout << "connect err" << endl;
        return -1;
    }
    cout << "connect success" << endl;
    return 0;
}

int TcpSocket::sendMsg(string msg)
{
    // 包头4字节（存储数据长度）
    char *pkg = new char[msg.size() + 4];
    // 将长度转化为网络序列
    int bigLen = htonl(msg.size());

    memcpy(pkg, &bigLen, 4);
    memcpy(pkg + 4, msg.data(), msg.size());

    int ret = writeN(pkg, msg.size() + 4);
    delete[] pkg;
    return ret;
}

string TcpSocket::recvMsg()
{
    int len = 0;
    readN((char *)&len, 4);
    len = ntohl(len);
    // cout << "pkg size: " << len << endl;

    char *buffer = new char[len + 1];
    int ret = readN(buffer, len);
    if (ret != len)
    {
        return string();
    }
    buffer[len] = '\0';

    string retStr(buffer);
    delete[] buffer;
    return retStr;
}

int TcpSocket::readN(char *buffer, int size)
{
    int n = 0;
    int count = size;
    char *p = buffer;

    while (count > 0)
    {
        if ((n = read(this->m_fd, p, count)) > 0)
        {
            p += n;
            count -= n;
        }
        else if (n == -1)
        {
            return -1;
        }
    }
    return size;
}

int TcpSocket::writeN(const char *msg, int size)
{
    int count = size;
    int n = 0;
    const char *p = msg;

    while (count > 0)
    {
        if ((n = write(this->m_fd, msg, count)) > 0)
        {
            p += n;
            count -= n;
        }
        else if (n == -1)
        {
            return -1;
        }
    }
    return size;
}
