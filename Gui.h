#ifndef CLIENT_GUI_H
#define CLIENT_GUI_H

#include "Client.h"
#include <iostream>
#include <fstream>
#include "data.h"

enum class GuiState {
    LoggedOut, LoggedIn
};

class Gui {
private:
    int socketFD;
    GuiState state;
public:
    Gui(int socketFD);

    bool showActions();
};

#endif //CLIENT_GUI_H