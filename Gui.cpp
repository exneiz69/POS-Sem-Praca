#include "Gui.h"

Gui::Gui(int socketFD) {
    this->socketFD = socketFD;
    this->state = GuiState::LoggedOut;
}

bool Gui::showActions() {
    if (this->state == GuiState::LoggedOut) {
        std::cout << "Actions available:" << std::endl;
        std::cout << "\t" << "1 : Login" << std::endl;
        std::cout << "\t" << "2 : Register" << std::endl;
        std::cout << "\t" << "0 : Exit" << std::endl;
        std::cout << "Enter choice: ";
        int choice = -1;

        while (true) {
            std::cin >> choice;

            if (choice == 0 || choice == 1 || choice == 2)
                break;
            else
                std::cout << "Invalid choice, enter again: ";
        }

        if (choice == 1) {
            userData user;
            std::cout << "Enter login: " << std::endl;
            std::cin >> user.login;

            std::cout << "Enter password: " << std::endl;
            std::cin >> user.password;

            Client::getInstance().encryptPassword(user);

            Reply reply = Client::getInstance().login(this->socketFD, user);

            if (reply == Reply::Success) {
                Client::getInstance().afterLoginSymetryPairing(this->socketFD);
                this->state = GuiState::LoggedIn;
                Client::getInstance().getNewEncryptedMessages(this->socketFD);
            }
            if (reply == Reply::Success)
                std::cout << "---Logged in---" << std::endl;
            else
                std::cout << "---Wrong username or password---" << std::endl;
        } else if (choice == 2) {
            userData user;
            std::cout << "Enter new login: " << std::endl;
            std::cin >> user.login;
            std::cout << "Enter new password: " << std::endl;
            std::cin >> user.password;

            Client::getInstance().encryptPassword(user);

            Reply reply = Client::getInstance().registerAccount(this->socketFD, user);

            if (reply == Reply::Success)
                std::cout << "---Account was created---" << std::endl;
            else
                std::cout << "---Account was not created---" << std::endl;
        } else if (choice == 0) {
            return false;
        }
    } else if (this->state == GuiState::LoggedIn) {
        std::cout << "\nActions available:" << std::endl;
        std::cout << "\t" << "1 : Get messages" << std::endl;
        std::cout << "\t" << "2 : Get new files" << std::endl;
        std::cout << "\t" << "3 : Get encrypted messages" << std::endl;
        std::cout << "\t" << "4 : Send message" << std::endl;
        std::cout << "\t" << "5 : Send encrypted message" << std::endl;
        std::cout << "\t" << "6 : Send file" << std::endl;
        std::cout << "\t" << "7 : Add friend" << std::endl;
        std::cout << "\t" << "8 : Get friend requests" << std::endl;
        std::cout << "\t" << "9 : Create group" << std::endl;
        std::cout << "\t" << "10 : Join group" << std::endl;
        std::cout << "\t" << "11 : History" << std::endl;
        std::cout << "\t" << "12 : Remove friend" << std::endl;
        std::cout << "\t" << "13 : Delete account" << std::endl;
        std::cout << "\t" << "14 : logout" << std::endl;
        std::cout << "\t" << "0 : Exit" << std::endl;
        std::cout << "Enter choice: ";
        int choice = -1;

        while (true) {
            std::cin.ignore(256, '\n');
            std::cin >> choice;

            if (choice < 15 && choice > -1)
                break;
            else
                std::cout << "Invalid choice, enter again: ";
        }

        if (choice == 1) {
            Reply reply = Client::getInstance().getNewMessages(this->socketFD);

            if (reply == Reply::Success)
                std::cout << "---All messages were read---" << std::endl;
            else
                std::cout << "---All messages were not read---" << std::endl;
        } else if (choice == 2) {
            Reply reply = Client::getInstance().getNewFiles(this->socketFD);

            if (reply == Reply::Success) {
                std::cout << "---Downloading file was successful---" << std::endl;
            } else
                std::cout << "---Downloading file was not successful---" << std::endl;

        } else if (choice == 3) {
            Reply reply = Client::getInstance().getNewEncryptedMessages(this->socketFD);

            if (reply == Reply::Success)
                std::cout << "---All encrypted messages were read---" << std::endl;
            else
                std::cout << "---All encrypted messages were not read---" << std::endl;
        } else if (choice == 4) {
            messageReducedData messageReducedData;
            std::cout << "Enter recipient: " << std::endl;
            std::cin >> messageReducedData.to;
            std::cout << "Enter message: " << std::endl;
            std::cin.ignore(256, '\n');
            std::cin.getline(messageReducedData.text, sizeof(messageReducedData::text));

            Reply reply = Client::getInstance().sendMessage(this->socketFD, messageReducedData);

            if (reply == Reply::Success)
                std::cout << "---Sent---" << std::endl;
            else
                std::cout << "---Not sent---" << std::endl;
        } else if (choice == 5) {
            messageReducedData messageReducedData;
            std::cout << "Enter recipient: " << std::endl;
            std::cin >> messageReducedData.to;
            std::cout << "Enter message: " << std::endl;
            std::cin.ignore(256, '\n');
            std::cin.getline(messageReducedData.text, sizeof(messageReducedData::text));

            Reply reply = Client::getInstance().sendEncryptedMessage(this->socketFD, messageReducedData);

            if (reply == Reply::Success)
                std::cout << "---Sent---" << std::endl;
            else
                std::cout << "---Not sent---" << std::endl;
        } else if (choice == 6) {
            fileReducedData fd;

            std::cout << "Enter recipient:" << std::endl;
            std::cin >> fd.to;
            std::cout << "Enter new filename:" << std::endl;
            std::cin >> fd.name;

            char pathToFile[256];
            std::cout << "Enter full path to file, with extension: " << std::endl;
            std::cin.ignore(256, '\n');
            std::cin.getline(pathToFile, 255);

            std::ifstream testInFile(pathToFile);
            int currentFileSize = 0;

            while (!testInFile.eof()) {
                char c;
                testInFile.get(c);
                fd.data[currentFileSize++] = c;
            }

            testInFile.close();

            std::cout << "---FILE---" << std::endl;
            for (int i = 0; i < currentFileSize; ++i) {
                std::cout << fd.data[i];
            }
            std::cout << "---FILE---" << std::endl;

            Reply reply = Client::getInstance().sendFile(this->socketFD, fd);

            if (reply == Reply::Success) {
                std::cout << "---Sending file was successful---" << std::endl;
            } else
                std::cout << "---Sending file was not successful---" << std::endl;

        } else if (choice == 7) {
            userData user;
            std::cout << "Enter friend's name: " << std::endl;
            std::cin >> user.login;

            Reply reply = Client::getInstance().addFriend(this->socketFD, user);

            if (reply == Reply::Success)
                std::cout << "---Friend request sent---" << std::endl;
            else
                std::cout << "---Friend request not sent---" << std::endl;
        } else if (choice == 8) {
            Reply reply = Client::getInstance().getFriendRequests(this->socketFD);
            if (reply == Reply::Success)
                std::cout << "---Friends added---" << std::endl;
            else
                std::cout << "---Friends not added---" << std::endl;
        } else if (choice == 9) {
            std::cout << "Enter group name: ";
            groupData gd;
            std::cin >> gd.name;

            Reply reply = Client::getInstance().createGroup(this->socketFD, gd);
            if (reply == Reply::Success)
                std::cout << "---Group created successfully---" << std::endl;
            else
                std::cout << "---Group not created---" << std::endl;
        } else if (choice == 10) {
            std::cout << "Enter group name: ";
            groupData gd;
            std::cin >> gd.name;
            Reply reply = Client::getInstance().addUserToGroup(this->socketFD, gd);
            if (reply == Reply::Success)
                std::cout << "---Added to group---" << std::endl;
            else
                std::cout << "---Not added to group---" << std::endl;
        } else if (choice == 11) {
            Reply reply = Client::getInstance().getHistory(this->socketFD);
            if (reply == Reply::Success)
                std::cout << "---History read---" << std::endl;
            else
                std::cout << "---History not read---" << std::endl;
        } else if (choice == 12) {
            userData user;
            std::cout << "Enter friend's name: " << std::endl;
            std::cin >> user.login;

            Reply reply = Client::getInstance().removeFriend(this->socketFD, user);

            if (reply == Reply::Success)
                std::cout << "---Friend removed---" << std::endl;
            else
                std::cout << "---Friend was not removed---" << std::endl;
        } else if (choice == 13) {
            Reply reply = Client::getInstance().deleteAccount(this->socketFD);

            if (reply == Reply::Success) {
                std::cout << "---Account was successfully deleted---" << std::endl;
                this->state = GuiState::LoggedOut;
            } else
                std::cout << "---Account was not successfully deleted---" << std::endl;
        } else if (choice == 14) {
            this->state = GuiState::LoggedOut;
            Reply reply = Client::getInstance().logout(this->socketFD);
        } else if (choice == 0) {
            return false;
        }

    }

    return true;
}
