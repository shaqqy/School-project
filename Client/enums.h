#ifndef ENUMS_H
#define ENUMS_H

enum class SchoolSkipper {
    Gamemode_Singleplayer = 1,
    Gamemode_Multiplayer = 2,
    /*
     * Size parameters
     */
    CHAT_WINDOW_WIDTH = 250,
    CHAT_INPUT_BOX_HEIGHT = 40,
    CHAT_EXPAND_BUTTON_Y_POS = 30,

    CUSTOM_MENU_BAR_HEIGHT = 30,

    /*
     *  Message types
     */
    INCOMING_MESSAGE = 187,
    OUTGOING_MESSAGE = 188,
};

#endif // ENUMS_H
