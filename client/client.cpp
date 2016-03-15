#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

class myTCP {
    struct sockaddr_in server;
    int socketDescriptor = 0;

public:
    myTCP(const char* address, int port) {
        server.sin_family = PF_INET;
        server.sin_addr.s_addr = inet_addr(address);        /* Comvert to Binary */
        server.sin_port = htons(port);                      /* Comvert to Binary */
        socketDescriptor = socket(PF_INET, SOCK_STREAM, 0); /* IPV4, TCP, No Flag */
    }

    bool connectServer() {
        return (connect(socketDescriptor, (struct sockaddr*)&server, sizeof(server))) == 0 ;
    }

    int sendMessages(char* buffer, int size) {
        return send(socketDescriptor, buffer, size, 0);
    }

    int readMessage(char* buffer, int size) {
        return read(socketDescriptor, buffer, size);
    }

    ~myTCP() {
        printf("closse connection" );
        close(socketDescriptor);
    }
};


int main() {

    char buffer[256] = {};
    char flag = 'y';
    myTCP tcp("127.0.0.1", 3500);
    tcp.connectServer();

    printf("Please input needed messages: \n");
    scanf ("%[^\n]%*c", buffer);
    printf("sending [%s] ...\n", buffer);
    tcp.sendMessages(buffer, sizeof(buffer));
    memset(buffer, 0, sizeof(buffer));
    tcp.readMessage(buffer, sizeof(buffer));
    printf("Server Reply = [%s]\n", buffer);

    return 0;
}
