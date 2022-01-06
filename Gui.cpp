//
// Created by Matej on 5. 1. 2022.
//

#include "Gui.h"


Gui::Gui(int socketFD) {
    this->socketFD = socketFD;
    this->state = GuiState::sLoggedOut;
}

bool Gui::showActions() {
    if (this->state == GuiState::sLoggedOut)
    {
        std::cout << "Actions available:" << std::endl;
        std::cout << "\t" << "1 : login" << std::endl;
        std::cout << "\t" << "2 : Register" << std::endl;
        std::cout << "\t" << "0 : Exit" << std::endl;
        std::cout << "Enter choice: ";
        int choice = -1;

        while(true) {
            std::cin >> choice;

            if (choice == 0 || choice == 1 || choice == 2)
                break;
            else
                std::cout << "Invalid choice, enter again: ";
        }

        if (choice == 1)
        {
            userData user;
            std::cout << "Enter login: " << std::endl;
            std::cin >> user.login;

            std::cout << "Enter password: " << std::endl;
            std::cin >> user.password;


            Reply reply = Client::getInstance().login(this->socketFD, user);

            if (reply == Reply::Success)
                this->state = GuiState::sLoggedIn;
        }
        else if (choice == 2)
        {
            userData user;
            std::cout << "Enter new login: " << std::endl;
            std::cin >> user.login; //sizeof(user.login)
            std::cout << "Enter new password: " << std::endl;
            std::cin >> user.password;

            Reply reply = Client::getInstance().registerAccount(this->socketFD, user);

            if (reply == Reply::Allowed)
                std::cout << "Allowed\n";
            else if (reply == Reply::Denied)
                std::cout << "Denied\n";
            else if (reply == Reply::Success)
                std::cout << "Success\n";
            else if (reply == Reply::Failure)
                std::cout << "Failure\n";
        }
        else if (choice == 0)
        {
            return false;
        }
    }
    else if (this->state == GuiState::sLoggedIn)
    {
        std::cout << "Actions available:" << std::endl;
        std::cout << "\t" << "1 : Get messages" << std::endl;
        std::cout << "\t" << "2 : Send messages" << std::endl;
        std::cout << "\t" << "3 : Add friend" << std::endl;
        std::cout << "\t" << "4 : Get friend requests" << std::endl;
        std::cout << "\t" << "5 : Remove friend" << std::endl;
        std::cout << "\t" << "6 : Delete account" << std::endl;
        std::cout << "\t" << "7 : logout" << std::endl;
        std::cout << "\t" << "0 : Exit" << std::endl;
        std::cout << "Enter choice: ";
        int choice = -1;

        while(true)
        {
            std::cin >> choice;

            if (choice < 8 && choice > -1)
                break;
            else
                std::cout << "Invalid choice, enter again: ";
        }

        if (choice == 1)
        {
            Reply reply = Client::getInstance().getNewMessages(this->socketFD);

            if (reply == Reply::Success)
                std::cout << "---All messages were read---" << std::endl;
            else
                std::cout << "---All messages were not read---" << std::endl;
        }
        else if (choice == 2)
        {
            messageReducedData message;
            std::cout << "Enter recipient: " << std::endl;
            std::cin >> message.to;
            std::cout << "Enter message: " << std::endl;
            std::cin >> message.text;

            Reply reply = Client::getInstance().sendMessage(this->socketFD, message);

            if (reply == Reply::Success)
                std::cout << "---Sent---" << std::endl;
            else
                std::cout << "---Not sent---" << std::endl;
        }
        else if (choice == 3)
        {
            userData user;
            std::cout << "Enter friend's name: " << std::endl;
            std::cin >> user.login;

            Reply reply = Client::getInstance().addFriend(this->socketFD, user);

            if (reply == Reply::Success)
                std::cout << "---Friend request sent---" << std::endl;
            else
                std::cout << "---Friend request not sent---" << std::endl;
        }
        else if (choice == 4)
        {
            Reply reply = Client::getInstance().getFriendRequests(this->socketFD);
            if (reply == Reply::Success)
                std::cout << "---Friends added---" << std::endl;
            else
                std::cout << "---Friends not added---" << std::endl;
        }
        else if (choice == 5)
        {
            userData user;
            std::cout << "Enter friend's name: " << std::endl;
            std::cin >> user.login;

            Reply reply = Client::getInstance().removeFriend(this->socketFD, user);

            if (reply == Reply::Success)
                std::cout << "---Friend removed---" << std::endl;
            else
                std::cout << "---Friend was not removed---" << std::endl;
        }
        else if (choice == 6)
        {
            Reply reply = Client::getInstance().deleteAccount(this->socketFD);

            if (reply == Reply::Success)
            {
                std::cout << "---Account was succesfully deleted---" << std::endl;
                this->state = GuiState::sLoggedOut;
            }
            else
                std::cout << "---Account was not succesfully deleted---" << std::endl;

        }
        else if (choice == 7)
        {
            this->state = GuiState::sLoggedOut;
            Client::getInstance().logout(this->socketFD);

        }
        else if (choice == 0)
        {
            return false;
        }

    }

    return true;
}
