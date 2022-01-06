#include "Client.h"

#include <unistd.h>
#include <iostream>

Reply Client::registerAccount(const int socketFD, userData newUser) {
    std::cout<<"registerAccount"<<std::endl;

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

    std::cout<<"REPLY " << (int)reply<<std::endl;

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

    if (reply == Reply::Allowed)
    {
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

    return reply;
}

Reply Client::sendMessage(const int socketFD, messageReducedData message) {
    Reply reply;
    reply = this->sendAction(socketFD, Action::SendMessage);

    if (reply == Reply::Allowed)
    {
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
            std::cout<<"From: "<<newMessage.from<<" To: "<<newMessage.to<<" Text: " <<newMessage.text << std::endl;
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

    if (reply == Reply::Allowed)
    {
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
            std::cout<<"User " << user.login << " wants to be your friend."<<std::endl;
            std::cout<<"Accept/Decline 1/0 : ";
            std::cin >> input;
            if (input == 1)
            {
                reply = Reply::Agree;
                std::cout<<"Friendship with " << user.login << " is successfully created"<<std::endl;
            }
            else
            {
                reply = Reply::Disagree;
                std::cout<<"Friendship with " << user.login << " is denied"<<std::endl;
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

    if (reply == Reply::Allowed)
    {
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
        std::cout<<"\\\\\\History---"<<std::endl;
        for (int i = 0; i < historyLinesNumber; i++) {
            messageData message;
            n = read(socketFD, &message, sizeof(messageData));
            if (n < 0) {
                perror("Error reading from socket");
            }
            std::cout<<"From: "<<message.from<<" To: "<< message.to << " Text: " << message.text<<std::endl;
        }
        std::cout<<"---History///"<<std::endl;

        n = read(socketFD, &reply, sizeof(Reply));
        if (n < 0) {
            perror("Error reading from socket");
        }
    }

    return reply;
}
