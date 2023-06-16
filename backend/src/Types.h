#ifndef TYPES_H_GUARD
#define TYPES_H_GUARD

enum SteamHotkeys
{
    STEAM = 0,
    QUICKMENU = 1,
    NUMBER_OF_STEAM_HOTKEYS
};

struct ModifierKeys
{
    bool meta{false};
    bool alt{false};
    bool ctrl{false};
    bool shift{false};
};

struct Hotkey
{
    ModifierKeys modifiers;
    unsigned short key{0};
};

#endif