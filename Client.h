#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "data.h"

class Client {
public:
    static Client &getInstance() {
        static Client instance;
        return instance;
    }

    Reply RegisterAccount(const int socketFD);

    Reply DeleteAccount(const int socketFD);

    Reply Login(const int socketFD);

    Reply Logout(const int socketFD);

    Reply SendMessage(const int socketFD);

    Reply GetNewMessages(const int socketFD);

private:
    Client() {}

    Reply sendAction(const int socketFD, Action action);

public:
    Client(Client const &) = delete;

    void operator=(Client const &) = delete;
};


#endif //CLIENT_CLIENT_H
