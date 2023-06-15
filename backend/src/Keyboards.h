#ifndef KEYBOARDS_H_GUARD
#define KEYBOARDS_H_GUARD

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <linux/input.h>

#include "Types.h"
#include "InputDevice.h"
#include "DeviceDataReader.h"

class Keyboard : public InputDevice
{
private:
    std::vector<Hotkey> hotkeys{SteamHotKeys::NUMBER_OF_STEAM_HOTKEYS};
    int event_number;
    int error_number;
    bool is_open{false};
    struct Modifiers
    {
        ModifierKeys left;
        ModifierKeys right;
    } modifiers;

public:
    Keyboard(int event_number = -1);
    ~Keyboard();

    bool openDevice();
    void closeDevice();
    std::optional<input_event> readEvent();
    void setFlags(input_event &ev);
    void setHotkey(const SteamHotKeys hotkey_type, const bool meta, const bool alt, const bool ctrl, const bool shift, const unsigned short ev_key);
    bool testModifiers(const SteamHotKeys hotkey_type);
    bool isHotkeyPressed(const SteamHotKeys hotkey_type);
};

class Keyboards
{
private:
    std::unordered_map<std::string, Keyboard> keyboards;
    DeviceDataReader reader;
    std::vector<std::string> active_keyboards;
    const std::string default_keyboard_name = "AT Translated Set 2 keyboard";

public:
    Keyboards(/* args */);
    ~Keyboards();
    std::vector<std::string> &getKeyboards();
    Keyboard &getKeyboard(const std::string &name);
    bool process(const SteamHotKeys hotkey_type);
};

#endif