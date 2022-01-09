#include "Client.h"

#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <stdlib.h>

Reply Client::registerAccount(const int socketFD, userData newUser) {
    std::cout << "registerAccount" << std::endl;

    Reply reply;

    reply = this->sendAction(socketFD, Action::RegisterAccount);

    if (reply == Reply::Allowed) {
//        userData newUser;
//        std::cin.getline(newUser.login, sizeof(newUser.login));
//        std::cin.getline(newUser.password, sizeof(newUser.password));

        int n;
        n = write(socketFD, &newUser, sizeof(userData));
        if (n < 0) {
            perror("Error writing to socket");
        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }

    std::cout << "REPLY " << (int) reply << std::endl;

    return reply;
}

Reply Client::deleteAccount(const int socketFD) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::DeleteAccount);

    if (reply == Reply::Allowed) {
        reply = Reply::Agree;

        int n;
        n = write(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error writing to socket");
        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }

    return reply;
}

Reply Client::login(const int socketFD, userData user) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::Login);

    if (reply == Reply::Allowed) {
        int n;
        n = write(socketFD, &user, sizeof(userData));
        if (n < 0) {
            perror("Error writing to socket");
        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }
    return reply;
}

Reply Client::logout(const int socketFD) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::Logout);

    if (reply == Reply::Allowed) {
        reply = Reply::Agree;

        int n;
        n = write(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error writing to socket");
        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }
    this->privateKey = 0;
    std::cout << (int)reply << std::endl;
    return reply;
}

Reply Client::sendMessage(const int socketFD, messageReducedData message) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::SendMessage);

    if (reply == Reply::Allowed) {
        int n;

        n = write(socketFD, &message, sizeof(messageReducedData));
        if (n < 0) {
            perror("Error writing to socket");
        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }

    return reply;
}

Reply Client::getNewMessages(const int socketFD) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::GetNewMessages);

    if (reply == Reply::Allowed) {
        int n;
        int newMessagesNumber;
        n = read(socketFD, &newMessagesNumber, sizeof(int));
        if (n < 0) {
            perror("Error reading from socket");
        }

        messageData newMessage;
        for (int i = 0; i < newMessagesNumber; i++) {
            n = read(socketFD, &newMessage, sizeof(messageData));
            if (n < 0) {
                perror("Error reading from socket");
            }
            std::cout << "From: " << newMessage.from << " To: " << newMessage.to << " Text: " << newMessage.text
                      << std::endl;
        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }

    return reply;
}

Reply Client::sendAction(const int socketFD, Action action) {
    int n;
    n = write(socketFD, &action, sizeof(Action));
    if (n < 0) {
        perror("Error writing to socket");
    }

    Reply reply;
    n = read(socketFD, &reply, sizeof(Reply));
    if (n < 0) {
        perror("Error reading from socket");
    }
    return reply;
}

Reply Client::addFriend(const int socketFD, userData user) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::AddFriend);

    if (reply == Reply::Allowed) {
        int n;
        n = write(socketFD, &user, sizeof(userData));
        if (n < 0) {
            perror("Error writing to socket");
        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }

    return reply;
}

Reply Client::getFriendRequests(const int socketFD) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::GetFriendRequests);

    if (reply == Reply::Allowed) {
        int n;
        int friendRequestsNumber;
        n = read(socketFD, &friendRequestsNumber, sizeof(int));
        if (n < 0) {
            perror("Error reading from socket");
        }

        for (int i = 0; i < friendRequestsNumber; i++) {
            userData user;
            n = read(socketFD, &user, sizeof(userData::login));
            if (n < 0) {
                perror("Error reading from socket");
            }

            int input = 0;
            std::cout << "User " << user.login << " wants to be your friend." << std::endl;
            std::cout << "Accept/Decline 1/0 : ";
            std::cin >> input;
            if (input == 1) {
                reply = Reply::Agree;
                std::cout << "Friendship with " << user.login << " is successfully created" << std::endl;
            } else {
                reply = Reply::Disagree;
                std::cout << "Friendship with " << user.login << " is denied" << std::endl;
            }


            int n;
            n = write(socketFD, &reply, sizeof(Reply));
            if (n < 0) {
                perror("Error writing to socket");
            }
        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }

    return reply;
}

Reply Client::removeFriend(const int socketFD, userData user) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::RemoveFriend);

    if (reply == Reply::Allowed) {
        int n;
        n = write(socketFD, &user, sizeof(userData));
        if (n < 0) {
            perror("Error writing to socket");
        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }

    return reply;
}

Reply Client::sendFile(const int socketFD, fileReducedData file) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::SendFile);
    std::cout << "trying to send SENDFILE action" << std::endl;
    if (reply == Reply::Allowed) {
        std::cout << "reply was allowed" << std::endl;
        int n;
        n = write(socketFD, &file, sizeof(fileReducedData));
        std::cout << "error 1" << std::endl;
        if (n < 0) {
            perror("Error writing to socket");
        }

        std::cout << "error 2" << std::endl;
        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }
    return reply;
}

Reply Client::getHistory(const int socketFD) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::GetHistory);

    if (reply == Reply::Allowed) {
        int n;
        int historyLinesNumber;
        n = read(socketFD, &historyLinesNumber, sizeof(int));
        if (n < 0) {
            perror("Error reading from socket");
        }
        std::cout << "\\\\\\History---" << std::endl;
        for (int i = 0; i < historyLinesNumber; i++) {
            messageData message;
            n = read(socketFD, &message, sizeof(messageData));
            if (n < 0) {
                perror("Error reading from socket");
            }
            std::cout << "From: " << message.from << " To: " << message.to << " Text: " << message.text << std::endl;
        }
        std::cout << "---History///" << std::endl;

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }
    return reply;
}


// Presun public casti budovania kluca zo servra do klienta
Reply Client::getPublicKey(const int socketFD) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::SendPublicKey);

    if (reply == Reply::Allowed) {
        int n;
        long long PublicP;
        n = read(socketFD, &PublicP, sizeof(long long));
        if (n < 0) {
            perror("Error reading from socket");
        }
        this->P = PublicP;
        reply = Reply::Agree;

        n = write(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
        long long PublicG;
        n = read(socketFD, &PublicG, sizeof(long long));
        if (n < 0) {
            perror("Error reading from socket");
        }

        this->G = PublicG;

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }

    return reply;
}

Reply Client::getNewFiles(const int socketFD) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::GetNewFiles);

    if (reply == Reply::Allowed) {
        int n;
        int newFilesNumber;
        n = read(socketFD, &newFilesNumber, sizeof(int));
        if (n < 0) {
            perror("Error reading from socket");
        }

        fileData newFile;
        for (int i = 0; i < newFilesNumber; i++) {
            n = read(socketFD, &newFile, sizeof(fileData));
            if (n < 0) {
                perror("Error reading from socket");
            }
            std::cout << "From: " << newFile.from << " To: " << newFile.to << " filename: " << newFile.name
                      << std::endl;

            char pathToFile[256];
            std::cout << "Enter full path to newly added file, with extension: " << std::endl;
            std::cin.ignore(256, '\n');
            std::cin.getline(pathToFile, 255);

            std::ofstream outFile(pathToFile);
            for (int j = 0; j < sizeof(fileData::data); ++j) {
                outFile << newFile.data[j];
            }
            outFile.close();

        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }
    return reply;
}


long long Client::diffieHelmanStepOne(long long Prime) {
    long long k = Prime;
    long long g = this->getG();
    long long p = this->getP();
    long long temp = ((g^k) % p);
//    std::cout << " Diffie 1 = " << temp << std::endl;
    return temp;
}

long long Client::diffieHelmanStepTwo(long long privateKeyComponentServer, long long privateKeyBase) {
    long long g = this->getG();
    long long p = this->getP();
    long long temp = ((privateKeyComponentServer) ^ privateKeyBase) % p;
//    std::cout << " Diffie 2 = " << temp << std::endl;
    return temp;
}


Reply Client::buildSymmetricConnection(const int socketFD) {
//    std::cout << " Zpusteny BSC" << std::endl;
    long long privateKeyBase = primeNumberGenerator();
    long long privateKeyComponentClient;
    long long privateKeyComponentServer;
    Reply reply;
    reply = this->sendAction(socketFD, Action::BuildSymmetricConnection);
    std::cout << (int) reply;
    privateKeyComponentClient = diffieHelmanStepOne(privateKeyBase);
    if (reply == Reply::Allowed) {
//        std::cout << "Idem do servra " << privateKeyComponentClient <<std::endl;
        int n;
        n = write(socketFD, &privateKeyComponentClient, sizeof(long long));
        if (n < 0) {
            perror("Error reading from socket");
        }

        reply = Reply::Agree;
        n = write(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error writing to socket");
        }

//        std::cout << "Idem od servera" << std::endl;
        n = read(socketFD, &privateKeyComponentServer, sizeof(long long));
        if (n < 0) {
            perror("Error reading from socket");
        }

//        std::cout << "Zo servra prislo " << privateKeyComponentServer << std::endl;

        long long temp = diffieHelmanStepTwo(privateKeyComponentServer, privateKeyBase);
        this->privateKey = temp;
        if (this->privateKey != 0) {
//            std::cout << "Success, Private key je vytvoreny." << std::endl;
        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error writing to socket");
        }
    }
    return reply;
}

long long Client::getP() {
    long long temp = this->P;
    return temp;
}

long long Client::getG() {
    long long temp = this->G;
    return temp;
}

void Client::afterLoginSymetryPairing(const int socketFD) {
    Client::getInstance().getPublicKey(socketFD);
//    std::cout << "P = " << this->getP() << " G = " << this->getG() << std::endl;
    Client::getInstance().buildSymmetricConnection(socketFD);
//    std::cout << "private key = " << this->privateKey << std::endl;
}


long long Client::primeNumberGenerator() {
    long long randomBeginning = ((rand()%10000)+ 10000) - (rand()%10000);
    long long primeNum = randomBeginning;
    bool isPrime = false;
//    std::cout << "Started searching for a sufficient prime, beginning is " << randomBeginning << std::endl;
    while (isPrime == false) {
        for (long long i = 2; i <= primeNum / 2; ++i) {
            if (primeNum % i == 0) {
                isPrime = false;
                break;
            }
            isPrime = true;
        }
        ++primeNum;
    }
    --primeNum;
//    std::cout << "Client has found a sufficient prime, " << primeNum << std::endl;

    return primeNum;
}

long long int Client::getPrivateKey() const {
    long long temp = this->privateKey;
    return temp;
}

Reply Client::sendEncryptedMessage(const int socketFD, messageReducedData message) {
    messageReducedData decryptedMessage = message;
    messageReducedData encryptedMessage = message;
    std::cout<< "Decrypted message: to: " << decryptedMessage.to << "; Text: " << decryptedMessage.text << std::endl;

    std::string to = decryptedMessage.to;
    std::string text = decryptedMessage.text;

    for (int i = 0; i < to.length(); ++i) {
        encryptedMessage.to[i] = (to[i] + (getPrivateKey() % 74));
    }
    for (int i = 0; i < text.length(); ++i) {
        encryptedMessage.text[i] = (text[i] + (getPrivateKey() % 74));
    }
    std::cout<< "Encrypted message: to: " << encryptedMessage.to << "; Text: " << encryptedMessage.text << std::endl;
    Reply reply;
    reply = this->sendAction(socketFD, Action::SendEncryptedMessage);
    if (reply == Reply::Allowed) {
        int n;
        n = write(socketFD, &encryptedMessage, sizeof(messageReducedData));
        if (n < 0) {
            perror("Error writing to socket");
        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }

    return reply;
}

Reply Client::getNewEncryptedMessages(const int socketFD) {
        Reply reply;
        reply = this->sendAction(socketFD, Action::GetEncryptedMessages);

        if (reply == Reply::Allowed) {
            int n;
            int newMessagesNumber;
            n = read(socketFD, &newMessagesNumber, sizeof(int));
            if (n < 0) {
                perror("Error reading from socket");
            }
            std::cout << "You have " << newMessagesNumber << " new encrypted messages." << std::endl;

            if (newMessagesNumber != 0) {
                messageData decryptedMessage;
                messageData encryptedMessage;
                messageData newMessage;
                for (int i = 0; i < newMessagesNumber; i++) {
                    n = read(socketFD, &newMessage, sizeof(messageData));
                    if (n < 0) {
                        perror("Error reading from socket");
                    }

                    encryptedMessage = newMessage;
                    std::cout << "Encrypted message: From: " << encryptedMessage.from << " to: " << encryptedMessage.to<< " Text: " << encryptedMessage.text << std::endl;

                    std::string from = encryptedMessage.from;
                    std::string to = encryptedMessage.to;
                    std::string text = encryptedMessage.text;

                    for (int i = 0; i < from.length(); ++i) {
                        decryptedMessage.from[i] = (from[i] - (getPrivateKey() % 74));
                    }
                    for (int i = 0; i < to.length(); ++i) {
                        decryptedMessage.to[i] = (to[i] - (getPrivateKey() % 74));
                    }
                    for (int i = 0; i < text.length(); ++i) {
                        decryptedMessage.text[i] = (text[i] - (getPrivateKey() % 74));
                    }
                    std::cout << "Decrypted message: From: " << decryptedMessage.from << " to: " << decryptedMessage.to << " Text: " << decryptedMessage.text << std::endl;
                }

                n = read(socketFD, &reply, sizeof(Reply));
                if (n < 0) {
                    perror("Error reading from socket");
                }
            }
            else{
                std::cout << " Nemate ziadne nove encrypted messages. "<< std::endl;
            }
        }
        return reply;
    }
