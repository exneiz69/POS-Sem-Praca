#include "Client.h"

#include <unistd.h>
#include <iostream>
#include <cstdio>

Reply Client::registerAccount(const int socketFD, const userData& newUser) {
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

Reply Client::login(const int socketFD, const userData& user) {
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

Reply Client::sendMessage(const int socketFD, const messageReducedData& message) {
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

Reply Client::addFriend(const int socketFD, const userData& user) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::AddFriend);

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

Reply Client::removeFriend(const int socketFD, const userData& user) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::RemoveFriend);

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

Reply Client::sendFile(const int socketFD, const fileReducedData& file) {
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

Reply Client::createGroup(const int socketFD, const groupData& group) {
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

Reply Client::addUserToGroup(const int socketFD, const groupData& group) {
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