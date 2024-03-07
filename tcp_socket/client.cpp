#include <iostream>
#include "TcpSocket.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;

int main()
{
    TcpSocket tcp;
    if (tcp.connectToHost("127.0.0.1", 9999))
    {
        return -1;
    }

    int file = open("message.txt", O_RDONLY);
    int len = 0;
    char tmp[100];
    memset(tmp, 0, sizeof(tmp));

    while ((len = read(file, tmp, sizeof(tmp))) > 0)
    {
        tcp.sendMsg(string(tmp, len));

        cout << "-----sendMsg-----" << endl;
        cout << tmp << endl;
        cout << endl;

        memset(tmp, 0, sizeof(tmp));

        usleep(300);
    }
    sleep(5);

    return 0;
}
