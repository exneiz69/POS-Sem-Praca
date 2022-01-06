#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "data.h"
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

    Reply getNewMessages(const int socketFD);
        /*TODO Pri pridavani frienda by sa mali zamenit komponenty symetrickych klucov na podporu encrypted direct messaging
         * z ktorych sa potom posklada symetricky private key pomocov random cisla pre kazdeho z dvoch friend userov. Ten bude potom
         * pouzity pre encrypciu a decrypciu messagov. SEE: Diffie-Hellman algorithm!
        */
    Reply addFriend(const int socketFD, userData user);

    Reply removeFriend(const int socketFD, userData user);

    Reply getFriendRequests(const int socketFD);
        //Malo by sa pouzivat pri vytvarani aby sa dostali oba public parametre do clienta co najkor.
    Reply getPublicKey(const int socketFD);
private:
    long long P = 0;
    int G = 0;
    int privateKey;
    Client() {}

    Reply sendAction(const int socketFD, Action action);
    //Todo bude pracovat len pri posielani messagov medzi friendmi
    std::string encryptMessage(std::string UnencryptedMessage);
    std::string decryptMessage(std::string EncryptedMessage);
public:
    Client(Client const &) = delete;

    void operator=(Client const &) = delete;
};


#endif //CLIENT_CLIENT_H
