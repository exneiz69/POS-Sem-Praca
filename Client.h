#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "data.h"
#include <iostream>
#include <fstream>
#include <string>

class Client {
public:
    static Client &getInstance() {
        static Client instance;
        return instance;
    }

    Reply registerAccount(int socketFD, const userData &newUser);

    Reply deleteAccount(int socketFD);

    Reply login(int socketFD, const userData &user);

    Reply logout(int socketFD);

    Reply sendMessage(int socketFD, const messageReducedData &message);

    Reply sendFile(int socketFD, const fileReducedData &file);

    Reply getNewMessages(int socketFD);

    Reply sendEncryptedMessage(int socketFD, const messageReducedData &message);

    Reply getNewEncryptedMessages(int socketFD);

    Reply getPublicKey(int socketFD);

    Reply buildSymmetricConnection(int socketFD);

    Reply getNewFiles(int socketFD);

    Reply addFriend(int socketFD, const userData &user);

    Reply removeFriend(int socketFD, const userData &user);

    Reply getFriendRequests(int socketFD);

    Reply getHistory(int socketFD);

    Reply createGroup(int socketFD, const groupData &group);

    Reply addUserToGroup(int socketFD, const groupData &group);

    void afterLoginSymetryPairing(int socketFD);

    void encryptPassword(userData& user);

private:
    long long P = 0;

    long long G = 0;

    long long privateKey = 0;

    Client() {}

    long long getP() const;

    long long getG() const;

    long long getPrivateKey() const;

    long long primeNumberGenerator();

    long long diffieHelmanStepOne(long long prime);

    long long diffieHelmanStepTwo(long long privateKeyComponentClient, long long privateKeyComponentServer);

    Reply sendAction(int socketFD, Action action);

public:
    Client(Client const &) = delete;

    void operator=(Client const &) = delete;
};

#endif //CLIENT_CLIENT_H