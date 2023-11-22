#ifndef KEYBOARDS_H_GUARD
#define KEYBOARDS_H_GUARD

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <memory>
#include <functional>
#include <linux/input.h>

#include "Parent.h"
#include "Types.h"
#include "InputDevice.h"
#include "DeviceDataReader.h"

class Preferences;
class Keyboard : public InputDevice
{
private:
    std::vector<HotkeyGroup> hotkeys{HotkeyGroupsEnum::NUMBER_OF_HOTKEYS};
    int event_number;
    int error_number;
    bool is_open{false};
    struct Modifiers
    {
        ModifierKeys left;
        ModifierKeys right;
    } modifiers;
    void openDevice();
    void closeDevice();

public:
    Keyboard(int event_number = -1);
    ~Keyboard();
    bool isEnabled();
    void setEnabled(bool enabled);
    std::optional<input_event> readEvent();
    void setFlags(input_event &ev);
    void setHotkeys(const HotkeyGroupsEnum hotkey_group, const bool alt, const bool ctrl, const bool shift, const bool meta, const unsigned short ev_key);
    void setHotkeys(const HotkeyGroupsEnum hotkey_group, const HotkeyGroup &hotkey);
    bool testModifiers(const HotkeyGroupsEnum hotkey_group);
    std::optional<HotkeyGroupsEnum> testHotkey();
    HotkeyGroup getHotkeys(const HotkeyGroupsEnum hotkey_group);
};

class Keyboards : public Parent
{
private:
    std::unordered_map<std::string, Keyboard> keyboards;
    DeviceDataReader reader;
    const std::vector<std::string> excluded_inputs = {"DP-1", "acp5x Headset Jack", "PC Speaker", "Valve Software Steam Deck Controller", "Video Bus", "Power Button", "AT Translated Set 2 keyboard"};

public:
    Keyboards();
    void stop();
    void reloadKeyboards();
    void saveKeyboard(const std::string &name, Keyboard &keyboard);
    std::vector<std::string> getKeyboardNames();
    void process();
    void setKeyboardEnabled(const std::string &name, bool enabled = true);
    bool isKeyboardEnabled(const std::string &name);
    void setKeyboardHotkeys(const std::string &name, const HotkeyGroupsEnum hotkey_group, const bool alt, const bool ctrl, const bool shift, const bool meta, const unsigned short ev_key);
    HotkeyGroup getKeyboardHotkeys(const std::string &name, const HotkeyGroupsEnum hotkey_group);
};

#endif