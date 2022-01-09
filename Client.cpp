#include "Client.h"

#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <cstring>

Reply Client::registerAccount(const int socketFD, const userData &newUser) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::RegisterAccount);

    if (reply == Reply::Allowed) {
        ssize_t n;
        n = write(socketFD, &newUser, sizeof(userData));
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

Reply Client::deleteAccount(const int socketFD) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::DeleteAccount);

    if (reply == Reply::Allowed) {
        int input = 0;
        std::cout << "Accept/Decline 1/0 : ";
        std::cin >> input;
        if (input == 1) {
            reply = Reply::Agree;
        } else {
            reply = Reply::Disagree;
        }

        ssize_t n;
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

Reply Client::login(const int socketFD, const userData &user) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::Login);

    if (reply == Reply::Allowed) {
        ssize_t n;
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

        ssize_t n;
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

Reply Client::sendMessage(const int socketFD, const messageReducedData &message) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::SendMessage);

    if (reply == Reply::Allowed) {
        ssize_t n;
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
        ssize_t n;
        int newMessagesNumber;
        n = read(socketFD, &newMessagesNumber, sizeof(int));
        if (n < 0) {
            perror("Error reading from socket");
        }
        std::cout << "You have " << newMessagesNumber << " new messages." << std::endl;

        messageData newMessage;
        for (int i = 0; i < newMessagesNumber; i++) {
            n = read(socketFD, &newMessage, sizeof(messageData));
            if (n < 0) {
                perror("Error reading from socket");
            }
            std::cout << "From: " << newMessage.from << " to: " << newMessage.to << " text: " << newMessage.text
                      << std::endl;
        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }

    return reply;
}

Reply Client::sendEncryptedMessage(const int socketFD, const messageReducedData &message) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::SendEncryptedMessage);
    if (reply == Reply::Allowed) {
        messageReducedData decryptedMessage = message;
        std::cout << "Decrypted message: to: " << decryptedMessage.to << "; Text: " << decryptedMessage.text
                  << std::endl;

        messageReducedData encryptedMessage;
        for (int i = 0; i < strlen(decryptedMessage.to); ++i) {
            encryptedMessage.to[i] = (decryptedMessage.to[i] + (getPrivateKey() % 74));
        }
        for (int i = 0; i < strlen(decryptedMessage.text); ++i) {
            encryptedMessage.text[i] = (decryptedMessage.text[i] + (getPrivateKey() % 74));
        }
        std::cout << "Encrypted message: to: " << encryptedMessage.to << "; Text: " << encryptedMessage.text
                  << std::endl;

        ssize_t n;
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
    reply = this->sendAction(socketFD, Action::GetNewEncryptedMessages);

    if (reply == Reply::Allowed) {
        ssize_t n;
        int newMessagesNumber;
        n = read(socketFD, &newMessagesNumber, sizeof(int));
        if (n < 0) {
            perror("Error reading from socket");
        }
        std::cout << "You have " << newMessagesNumber << " new encrypted messages." << std::endl;

        for (int i = 0; i < newMessagesNumber; i++) {
            messageData encryptedMessage;
            messageData decryptedMessage;

            n = read(socketFD, &encryptedMessage, sizeof(messageData));
            if (n < 0) {
                perror("Error reading from socket");
            }

            std::cout << "Encrypted message: From: " << encryptedMessage.from << " to: " << encryptedMessage.to
                      << " Text: " << encryptedMessage.text << std::endl;

            for (int j = 0; j < strlen(encryptedMessage.from); ++j) {
                decryptedMessage.from[j] = (encryptedMessage.from[j] - (getPrivateKey() % 74));
            }
            for (int j = 0; j < strlen(encryptedMessage.to); ++j) {
                decryptedMessage.to[j] = (encryptedMessage.to[j] - (getPrivateKey() % 74));
            }
            for (int j = 0; j < strlen(encryptedMessage.text); ++j) {
                decryptedMessage.text[j] = (encryptedMessage.text[j] - (getPrivateKey() % 74));
            }
            std::cout << "Decrypted message: From: " << decryptedMessage.from << " to: " << decryptedMessage.to
                      << " Text: " << decryptedMessage.text << std::endl;
        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }
    return reply;
}

Reply Client::getPublicKey(const int socketFD) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::GetPublicKey);

    if (reply == Reply::Allowed) {
        ssize_t n;
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

Reply Client::buildSymmetricConnection(const int socketFD) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::BuildSymmetricConnection);

    if (reply == Reply::Allowed) {
        long long privateKeyBase = primeNumberGenerator();
        long long privateKeyComponentClient;
        long long privateKeyComponentServer;
        privateKeyComponentClient = diffieHelmanStepOne(privateKeyBase);

        ssize_t n;
        n = write(socketFD, &privateKeyComponentClient, sizeof(long long));
        if (n < 0) {
            perror("Error reading from socket");
        }

        reply = Reply::Agree;
        n = write(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error writing to socket");
        }

        n = read(socketFD, &privateKeyComponentServer, sizeof(long long));
        if (n < 0) {
            perror("Error reading from socket");
        }

        long long temp = diffieHelmanStepTwo(privateKeyComponentServer, privateKeyBase);
        this->privateKey = temp;
        if (this->privateKey != 0) {

        }

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error writing to socket");
        }
    }
    return reply;
}

Reply Client::addFriend(const int socketFD, const userData &user) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::AddFriend);

    if (reply == Reply::Allowed) {
        ssize_t n;
        n = write(socketFD, &user, sizeof(userData::login));
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
        ssize_t n;
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

Reply Client::removeFriend(const int socketFD, const userData &user) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::RemoveFriend);

    if (reply == Reply::Allowed) {
        ssize_t n;
        n = write(socketFD, &user, sizeof(userData::login));
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

Reply Client::sendFile(const int socketFD, const fileReducedData &file) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::SendFile);

    if (reply == Reply::Allowed) {
        ssize_t n;
        n = write(socketFD, &file, sizeof(fileReducedData));
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

Reply Client::getHistory(const int socketFD) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::GetHistory);

    if (reply == Reply::Allowed) {
        ssize_t n;
        int historyLinesNumber;
        n = read(socketFD, &historyLinesNumber, sizeof(int));
        if (n < 0) {
            perror("Error reading from socket");
        }
        std::cout << "History---" << std::endl;
        for (int i = 0; i < historyLinesNumber; i++) {
            messageData message;
            n = read(socketFD, &message, sizeof(messageData));
            if (n < 0) {
                perror("Error reading from socket");
            }
            std::cout << "From: " << message.from << " To: " << message.to << " Text: " << message.text << std::endl;
        }
        std::cout << "---History" << std::endl;

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
        ssize_t n;
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
            std::cout << "From: " << newFile.from << " to: " << newFile.to << " file name: " << newFile.name
                      << std::endl;

            char pathToFile[256];
            std::cout << "Enter full path to newly added file, with extension: " << std::endl;
            std::cin.ignore(256, '\n');
            std::cin.getline(pathToFile, 255);

            std::ofstream outFile(pathToFile);
            for (char j: newFile.data) {
                outFile << j;
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

Reply Client::createGroup(const int socketFD, const groupData &group) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::CreateGroup);

    if (reply == Reply::Allowed) {
        ssize_t n;
        n = write(socketFD, &group, sizeof(groupData));
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

Reply Client::addUserToGroup(const int socketFD, const groupData &group) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::AddUserToGroup);
    if (reply == Reply::Allowed) {
        ssize_t n;
        n = write(socketFD, &group, sizeof(groupData));
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

void Client::afterLoginSymetryPairing(const int socketFD) {
    Client::getInstance().getPublicKey(socketFD);
    Client::getInstance().buildSymmetricConnection(socketFD);
}

long long Client::getP() const {
    return this->P;
}

long long Client::getG() const {
    return this->G;
}

long long Client::getPrivateKey() const {
    return this->privateKey;
}

long long Client::primeNumberGenerator() {
    long long randomBeginning = ((rand() % 10000) + 10000) - (rand() % 10000);
    long long primeNum = randomBeginning;
    bool isPrime = false;
    while (!isPrime) {
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

    return primeNum;
}

long long Client::diffieHelmanStepOne(long long prime) {
    long long k = prime;
    long long g = this->getG();
    long long p = this->getP();
    long long temp = ((g ^ k) % p);
    return temp;
}

long long Client::diffieHelmanStepTwo(long long privateKeyComponentServer, long long privateKeyBase) {
    long long g = this->getG();
    long long p = this->getP();
    long long temp = ((privateKeyComponentServer) ^ privateKeyBase) % p;
    return temp;
}

Reply Client::sendAction(const int socketFD, Action action) {
    ssize_t n;
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

void Client::encryptPassword(userData &user) {
    std::string tempString;
    std::string encryptedPassword = "Dano";
    encryptedPassword += user.password;
    encryptedPassword += "Drevo";
    int messageLength = encryptedPassword.length();
    char temp;

    for (int j = 0; j < 80; ++j) {
        temp = encryptedPassword[0];
        for (int i = 0; i < messageLength - 1; ++i) {
            encryptedPassword[i] = encryptedPassword[i + 1];
        }
        encryptedPassword[messageLength - 1] = temp;
    }

    for (int i = 0; i < messageLength; ++i) {
        encryptedPassword[i] = (char) (encryptedPassword[i] % 74);
        encryptedPassword[i] += (char) 128;
    }
    tempString = encryptedPassword;
    for (int i = messageLength - 1; i >= 0; --i) {
        (encryptedPassword).push_back(tempString[i]);
    }

    strcpy((char *) user.password, encryptedPassword.c_str());
}
