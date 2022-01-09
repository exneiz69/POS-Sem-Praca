#ifndef CLIENT_GUI_H
#define CLIENT_GUI_H


#include "Client.h"
#include <iostream>
#include <fstream>
#include "data.h"
#include <string.h>

enum class GuiState {
    LoggedOut, LoggedIn
};
//
//enum class GuiActions
//{
//    Exit=0, GetMessages, SendMessage, GetEncryptedMessages, SendEncryptedMessages, AddFriend, GetFriendRequests, RemoveFriend,
//};

class Gui {
private:
    int socketFD;
    GuiState state;
public:
    Gui(int socketFD);

    bool showActions();
};

#endif //CLIENT_GUI_H