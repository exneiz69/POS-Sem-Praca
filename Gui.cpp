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

            if (reply == Reply::Success) {
                Client::getInstance().afterLoginSymetryPairing(this->socketFD);
                this->state = GuiState::sLoggedIn;
                std::cout<<std::endl;
                std::cout << "Vypisuju sa encrypted message ktore vam boli zaslane kym bol uzivatel offline:" << std::endl;
                Client::getInstance().getNewEncryptedMessages(this->socketFD);
            }
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
        std::cout << "\t" << "2 : Send message" << std::endl;
        std::cout << "\t" << "3 : Send encrypted message" << std::endl;
        std::cout << "\t" << "4 : Add friend" << std::endl;
        std::cout << "\t" << "5 : Get friend requests" << std::endl;
        std::cout << "\t" << "6 : Remove friend" << std::endl;
        std::cout << "\t" << "7 : Delete account" << std::endl;
        std::cout << "\t" << "8 : logout" << std::endl;
        std::cout << "\t" << "9 : Send file" << std::endl;
        std::cout << "\t" << "10 : Get new files" << std::endl;
        std::cout << "\t" << "11 : Get history" << std::endl;
        std::cout << "\t" << "0 : Exit" << std::endl;
        std::cout << "Enter choice: ";
        int choice = -1;

        while(true)
        {
            std::cin >> choice;

            if (choice < 11 && choice > -1)
                break;
            else
                std::cout << "Invalid choice, enter again: ";
        }

        if (choice == 1)
        {
            Reply reply = Client::getInstance().getNewMessages(this->socketFD);

            if (reply == Reply::Success)
                std::cout << "---No new messages.---" << std::endl;
            else
                std::cout << "---Receiving new messages.---" << std::endl;
        }
        else if (choice == 2)
        {
            messageReducedData message;
            std::cout << "Enter recipient: " << std::endl;
            std::cin >> message.to;
            std::cout << "Enter message: " << std::endl;
            std::cin.ignore(256, '\n');
            messageData md;
            std::cin.getline(message.text, sizeof(md.text));

            Reply reply = Client::getInstance().sendMessage(this->socketFD, message);

            if (reply == Reply::Success)
                std::cout << "---Sent---" << std::endl;
            else
                std::cout << "---Not sent---" << std::endl;
        }
        else if (choice == 3)
        {
            messageReducedData message;
            std::cout << "Enter recipient: " << std::endl;
            std::cin >> message.to;
            std::cout << "Enter message: " << std::endl;
            std::cin.ignore(256, '\n');
            messageData md;
            std::cin.getline(message.text, sizeof(md.text));

            Reply reply = Client::getInstance().sendEncryptedMessage(this->socketFD, message);

            if (reply == Reply::Success)
                std::cout << "---Encrypted message sent---" << std::endl;
            else
                std::cout << "---Encrypted message not sent---" << std::endl;
        }
        else if (choice == 4)
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
        else if (choice == 5)
        {
            Reply reply = Client::getInstance().getFriendRequests(this->socketFD);
            if (reply == Reply::Success)
                std::cout << "---Friends added---" << std::endl;
            else
                std::cout << "---Friends not added---" << std::endl;
        }
        else if (choice == 6)
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
        else if (choice == 7)
        {
            Reply reply = Client::getInstance().deleteAccount(this->socketFD);

            if (reply == Reply::Success)
            {
                std::cout << "---Account was succesfully deleted---" << std::endl;
                this->state = GuiState::sLoggedOut;
            }
            else
                std::cout << "---Account was not successfully deleted---" << std::endl;

        }
        else if (choice == 8)
        {
            this->state = GuiState::sLoggedOut;
            Client::getInstance().logout(this->socketFD);

        }
        else if (choice == 9)
        {
            fileReducedData fd;

            std::cout<<"Enter recipient:"<<std::endl;
            std::cin >> fd.to;
            std::cout<<"Enter new filename:"<<std::endl;
            std::cin >> fd.name;

            char pathToFile[256];
            std::cout << "Enter full path to file, with extension: " << std::endl;
            std::cin.ignore(256, '\n');
            std::cin.getline(pathToFile, 255);

            std::ifstream testInFile(pathToFile);
            int maxFileSize = sizeof(fileReducedData::data);
            int currentFileSize = 0;

//            while (testInFile.peek() != EOF && currentFileSize < maxFileSize - 1) {
//                testInFile >> fd.data[currentFileSize++]; // fd.data ... char[]
//                char c = getc(testInFile);
//                fd.data[currentFileSize++] = c;
//                testInFile >> c; // posun ukazovatel
//            }
            while (!testInFile.eof())
            {
                char c;
                testInFile.get(c);
                fd.data[currentFileSize++] = c;
            }

            testInFile.close();

            std::cout<<"---FILE---"<<std::endl;
            for (int i = 0; i < currentFileSize; ++i) {
                std::cout<<fd.data[i];
            }
            std::cout<<"---FILE---"<<std::endl;

            Reply reply = Client::getInstance().sendFile(this->socketFD, fd);

            if (reply == Reply::Success)
            {
                std::cout << "---Sending file was successful---" << std::endl;
            }
            else
                std::cout << "---Sending file was not successful---" << std::endl;

        }
        else if (choice == 10)
        {
            Reply reply = Client::getInstance().getNewFiles(this->socketFD);

            if (reply == Reply::Success)
            {
                std::cout << "---Downloading file was successful---" << std::endl;
            }
            else
                std::cout << "---Downloading file was not successful---" << std::endl;

        }
        else if (choice == 11)
        {
            Reply reply = Client::getInstance().getHistory(this->socketFD);
            if (reply == Reply::Success)
                std::cout << "---History read---" << std::endl;
            else
                std::cout << "---History not read---" << std::endl;
        }
        else if (choice == 0)
        {
            return false;
        }

    }

    return true;
}
