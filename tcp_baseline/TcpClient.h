#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class TcpClient
{
public:
    TcpClient() : m_fd(-1){};
    TcpClient(string ip, int port);
    ~TcpClient();

    void connectToHost();
    void request();

private:
    int m_fd;
    string m_serverIP;
    int m_serverPort;
};

TcpClient::TcpClient(string ip, int port)
{
    this->m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->m_fd < 0)
    {
        perror("fd");
        exit(2);
    }
    this->m_serverIP = ip;
    this->m_serverPort = port;
}

TcpClient::~TcpClient()
{
    if (this->m_fd >= 0)
    {
        close(this->m_fd);
    }
}

void TcpClient::connectToHost()
{
    struct sockaddr_in peer;
    memset(&peer, 0, sizeof(peer));

    peer.sin_family = AF_INET;
    peer.sin_port = htons(this->m_serverPort);
    peer.sin_addr.s_addr = inet_addr(this->m_serverIP.c_str());

    if (connect(this->m_fd, (struct sockaddr *)&peer, sizeof(peer)) == 0)
    {
        cout << "connect success" << endl;
        request();
    }
    else
    {
        perror("connect failed");
        exit(3);
    }
}

void TcpClient::request()
{
    string msg;
    char buf[1024];

    while (true)
    {
        cout << "please enter#: ";
        getline(cin, msg);
        write(this->m_fd, msg.c_str(), msg.size());

        ssize_t size = read(this->m_fd, buf, sizeof(buf) - 1);

        if (size > 0)
        {
            buf[size] = '\0';
            cout << "server echo: " << buf << endl;
        }
        else if (size == 0)
        {
            cout << "server close" << endl;
            break;
        }
        else
        {
            cout << "read err" << endl;
            break;
        }
    }
}