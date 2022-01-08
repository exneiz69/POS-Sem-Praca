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

    Reply buildSymmetricConnection(const int socketFD);

    void afterLoginSymetryPairing(const int socketFD);


private:
    long long P = 0;
    long long G = 0;
    long long privateKey; // TODO iny pre kazdu session. Vytvori sa pri prihlaseni a zmaze sa pri odhlaseni.

    Client() {}

    Reply sendAction(const int socketFD, Action action);

    long long diffieHelmanStepOne(long long Prime);

    long long diffieHelmanStepTwo(long long privateKeyComponentClient, long long privateKeyComponentServer);

    long long getP();

    long long getG();

    //TODO samotna metoda ktora bude encryptovat messages.
    std::string encryptMessage(std::string UnencryptedMessage);
    //TODO samotna metoda ktora bude decryptovat messages.
    std::string decryptMessage(std::string EncryptedMessage);

public:
    Client(Client const &) = delete;

    void operator=(Client const &) = delete;
};


#endif //CLIENT_CLIENT_H
