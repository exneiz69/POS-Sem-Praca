#include "data.h"
#include "Client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]) {
    int socketFD;
    struct sockaddr_in serverAddress;
    struct hostent *server;

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        return 1;
    }

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy(
            (char *) server->h_addr,
            (char *) &serverAddress.sin_addr.s_addr,
            server->h_length
    );
    serverAddress.sin_port = htons(atoi(argv[2]));

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) {
        perror("Error creating socket");
        return 3;
    }

    if (connect(socketFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error connecting to socket");
        return 4;
    }

    Reply reply;
    reply = Client::getInstance().RegisterAccount(socketFD);

    if (reply == Reply::Allowed) {
        std::cout << "Allowed\n";
    } else if (reply == Reply::Denied) {
        std::cout << "Denied\n";
    } else if (reply == Reply::Success) {
        std::cout << "Success\n";
    } else if (reply == Reply::Failure) {
        std::cout << "Failure\n";
    }

    close(socketFD);

    return 0;
}
