#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "data.h"

class Client {
public:
    static Client &getInstance() {
        static Client instance;
        return instance;
    }

    Reply registerAccount(const int socketFD, userData newUser);

    Reply deleteAccount(const int socketFD);

    Reply login(const int socketFD, userData user);

    Reply logout(const int socketFD);

    Reply sendMessage(const int socketFD, messageReducedData message);

    Reply getNewMessages(const int socketFD);

    Reply addFriend(const int socketFD, userData user);

    Reply removeFriend(const int socketFD, userData user);

    Reply getFriendRequests(const int socketFD);

private:
    Client() {}

    Reply sendAction(const int socketFD, Action action);

public:
    Client(Client const &) = delete;

    void operator=(Client const &) = delete;
};


#endif //CLIENT_CLIENT_H
