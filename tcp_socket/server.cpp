#include <iostream>
#include <pthread.h>
#include "TcpServer.h"
using namespace std;

struct SockInfo
{
    TcpServer *s;
    TcpSocket *tcp;
    struct sockaddr_in addr;
};

void *working(void *arg)
{
    struct SockInfo *pInfo = static_cast<struct SockInfo *>(arg);

    char ip[32];
    cout << "clientIP: " << inet_ntop(AF_INET, &pInfo->addr.sin_addr.s_addr, ip, sizeof(ip)) << ", clientPort: " << ntohs(pInfo->addr.sin_port) << endl;

    while (1)
    {
        string msg = pInfo->tcp->recvMsg();
        if (!msg.empty())
        {
            cout << "----recvData----" << endl;
            cout << msg << endl;
            cout << endl;
        }
        else
        {
            break;
        }
    }
    delete pInfo->tcp;
    delete pInfo;
    return nullptr;
}

int main()
{
    TcpServer server;
    server.setListen(9999);

    while (1)
    {
        SockInfo *pInfo = new SockInfo;
        TcpSocket *tcp = server.acceptConn(&pInfo->addr);
        if (tcp == nullptr)
        {
            cout << "again" << endl;
            // 继续接受其他客户端的请求
            continue;
        }
        pInfo->s = &server;
        pInfo->tcp = tcp;

        pthread_t tid;
        pthread_create(&tid, nullptr, working, pInfo);
        pthread_detach(tid);
    }
    return 0;
}
