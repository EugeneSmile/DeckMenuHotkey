#ifndef TYPES_H_GUARD
#define TYPES_H_GUARD

enum HotkeyGroupsEnum
{
    STEAM = 0,
    QUICKMENU = 1,
    NUMBER_OF_HOTKEYS
};

struct ModifierKeys
{
    bool alt{false};
    bool ctrl{false};
    bool shift{false};
    bool meta{false};
};

struct HotkeyGroup
{
    ModifierKeys modifiers;
    unsigned short key{0};
};

#endif