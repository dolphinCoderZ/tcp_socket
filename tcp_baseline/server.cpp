#include <iostream>
#include "TcpServer.h"
using namespace std;

void printPort(string proc)
{
    cout << "use: " << proc << " port" << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printPort(argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    TcpServer *server = new TcpServer(port);
    server->initSever();
    server->startServer();
    return 0;
}
