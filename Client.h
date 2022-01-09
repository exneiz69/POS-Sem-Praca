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


    Reply registerAccount(const int socketFD, userData newUser);

    Reply deleteAccount(const int socketFD);

    Reply login(const int socketFD, userData user);

    Reply logout(const int socketFD);

    Reply sendMessage(const int socketFD, messageReducedData message);

    Reply sendFile(const int socketFD, fileReducedData file);

    Reply getNewMessages(const int socketFD);
  
    Reply getNewFiles(const int socketFD);

    Reply addFriend(const int socketFD, userData user);

    Reply removeFriend(const int socketFD, userData user);

    Reply getFriendRequests(const int socketFD);

    Reply getHistory(const int socketFD);

    Reply getPublicKey(const int socketFD);

    Reply sendEncryptedMessage(const int socketFD, messageReducedData message);

    Reply getNewEncryptedMessages(const int socketFD);

    void afterLoginSymetryPairing(const int socketFD);

    Reply createGroup(const int socketFD, groupData group);

    Reply addUserToGroup(const int socketFD, groupData group);

private:
    long long P = 0;
    long long G = 0;
    long long privateKey = 0;

private:

    long long int getPrivateKey() const;

    Client() {}

    Reply sendAction(const int socketFD, Action action);

    Reply buildSymmetricConnection(const int socketFD);

    long long diffieHelmanStepOne(long long Prime);

    long long diffieHelmanStepTwo(long long privateKeyComponentClient, long long privateKeyComponentServer);

    long long getP();

    long long getG();

    long long primeNumberGenerator();

public:
    Client(Client const &) = delete;

    void operator=(Client const &) = delete;
};


#endif //CLIENT_CLIENT_H