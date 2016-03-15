#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void error(const char *msg) {
 perror(msg);
 exit(1);
}

class myTCPServer {
    int serverSocket = 0;
    int clientSocket = 0;
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t addressSize;

public:
    myTCPServer(const char* address, int port) {
        server.sin_family = PF_INET;
        server.sin_addr.s_addr = inet_addr(address);
        server.sin_port = htons(port);
        serverSocket = socket(PF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            error("ERROR on creating socket");
        }

        if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) < 0) {
            error("ERROR on binding");
        }
        /*
            only for C (not useable in C++)
            ---------------------------------------------------
            struct sockaddr_in server = {
                .sin_family = PF_INET
                .sin_address.s_addr = inet_addr("127.0.0.1")
                .sin_port = htons(3500)
            };
        */
        listen(serverSocket, 5);
    }

    int acceptClient() {
        addressSize = sizeof(client);
        clientSocket = accept(serverSocket, (struct sockaddr*)&client, &addressSize);
        if (clientSocket < 0) {
            error("ERROR on accepting client");
        }

        return clientSocket;
    }

    int readMessage(char* buffer, int size) {
        int readSize = 0;
        readSize = recv(clientSocket, buffer, size, 0);
        printf("buffer = [%s]\n", buffer);
        buffer[readSize] = '\0';

        return readSize;
    }

    int sendMessages(char* buffer, int size) {
        send(clientSocket, buffer, size, 0);
    }

    void exitClient() {
        close(clientSocket);
    }

    ~myTCPServer() {
        close(serverSocket);
    }
};

void generateReply(char* buffer, int size) {
    char* token = NULL;
    int number[5] = {0};
    int min = 0;
    int max = 0;

    token = strtok(buffer, " ");
    for (int i=0; i < 5; i++) {
        if (token == NULL) {
            char errMsg[] = "ERROR, too few arguments";
            strncpy(buffer, errMsg, sizeof(errMsg)/sizeof(char));
            return;
        } else {
            number[i] = atoi(token);
            printf("number[%d] = %d\t", i,number[i] );
        }
        token = strtok(NULL, " ");
    }

    printf("\n");

    min = number[0];
    max = number[0];

    for (int i=0; i < 5; i++) {
        if (number[i] < min) {
            min = number[i];
        }
        if (number[i] > max) {
            max = number[i];
        }
    }

    memset(buffer, 0, size);
    sprintf(buffer, "MAX(%d), MIN(%d)", max, min);
}

int main() {

    int readSize = 0;
    char buffer[256] = {0};
    myTCPServer server("127.0.0.1", 3500);

    printf("Server Ready!\n");

    int clientSocket = server.acceptClient();
    printf("Client ID [%d] connected\n", clientSocket);

    while ((readSize = server.readMessage(buffer, sizeof(buffer))) > 0) {
        printf("read message [%s]\n", buffer);
        generateReply(buffer, sizeof(buffer));
        printf("sending Reply [%s]\n", buffer);
        server.sendMessages(buffer, sizeof(buffer));
    }

    if (readSize == 0) {
        printf("Client Disconnected, Shutting Down.\n");
        server.exitClient();
    } else if (readSize == -1) {
        error("ERROR, Something Happened!\n");
    }

    return 0;
}

/* a structure to contain an internet address defined in the include file in.h
    struct sockaddr_in {
        short   sin_family; // should be AF_INET
        u_short sin_port;   // unsigned short
        struct  in_addr sin_addr;
        char    sin_zero[8]; // not used, must be zero
    };
*/

/*
    The inet_addr() function converts the Internet host address cp from
    IPv4 numbers-and-dots notation into binary data in network byte order.
    If the input is invalid, INADDR_NONE (usually -1) is returned.
    Use of this function is problematic because -1 is a valid address (255.255.255.255).
    Avoid its use in favor of inet_aton(), inet_pton(3), or getaddrinfo(3)
    which provide a cleaner way to indicate error return.
*/

/*
        int socket(int domain, int type, int protocol);

        On success, a file descriptor for the new socket is returned.  On
        error, -1 is returned, and errno is set appropriately.

        AF_UNIX, AF_LOCAL   Local communication              unix(7)
        AF_INET             IPv4 Internet protocols          ip(7)
        AF_INET6            IPv6 Internet protocols          ipv6(7)
        AF_IPX              IPX - Novell protocols
        AF_NETLINK          Kernel user interface device     netlink(7)
        AF_X25              ITU-T X.25 / ISO-8208 protocol   x25(7)
        AF_AX25             Amateur radio AX.25 protocol
        AF_ATMPVC           Access to raw ATM PVCs
        AF_APPLETALK        AppleTalk                        ddp(7)
        AF_PACKET           Low level packet interface       packet(7)
        AF_ALG              Interface to kernel crypto API

        SOCK_STREAM     Provides sequenced, reliable, two-way, connection-
                        based byte streams.  An out-of-band data transmission
                        mechanism may be supported.

        SOCK_DGRAM      Supports datagrams (connectionless, unreliable
                        messages of a fixed maximum length).

        SOCK_SEQPACKET  Provides a sequenced, reliable, two-way connection-
                        based data transmission path for datagrams of fixed
                        maximum length; a consumer is required to read an
                        entire packet with each input system call.

        SOCK_RAW        Provides raw network protocol access.

        SOCK_RDM        Provides a reliable datagram layer that does not
                        guarantee ordering.

        SOCK_PACKET     Obsolete and should not be used in new programs; see
                        packet(7).
*/
