#include <iostream>
#include "TcpClient.h"
using namespace std;

void printArgu(string proc)
{
    cout << "use: " << proc << "serverIP serverPort" << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printArgu(argv[0]);
        exit(1);
    }

    string serverIP = argv[1];
    int serverPort = atoi(argv[2]);

    TcpClient *client = new TcpClient(serverIP, serverPort);

    client->connectToHost();

    return 0;
}
