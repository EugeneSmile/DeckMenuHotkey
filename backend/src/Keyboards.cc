#include "Keyboards.h"

#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <algorithm>

#include "Manager.h"
#include "Preferences.h"

Keyboard::Keyboard(int event_number) : event_number(event_number)
{
}

Keyboard::~Keyboard()
{
    closeDevice();
}

bool Keyboard::isEnabled()
{
    return is_open;
}

void Keyboard::setEnabled(bool enabled)
{
    if (is_open && !enabled)
        closeDevice();
    else if (!is_open && enabled)
        openDevice();
}

void Keyboard::openDevice()
{
    std::string event_path = "/dev/input/event" + std::to_string(event_number);
    fd = open(event_path.c_str(), O_RDONLY | O_NONBLOCK);
    if (fd == -1)
    {
        error_number = errno;
        fprintf(stderr, "[deckmenuhotkey] Cannot open %s: %s.\n", event_path, strerror(errno));
        return;
    }
    is_open = true;
}

void Keyboard::closeDevice()
{
    if (is_open)
        close(fd);
    is_open = false;
}

std::optional<input_event> Keyboard::readEvent()
{
    if (is_open)
    {
        input_event ev;

        ssize_t n;
        n = read(fd, &ev, sizeof ev);
        if (n == (ssize_t)-1)
        {
            if (errno == EAGAIN)
            {
                return {};
            }
            if (errno != EINTR)
            {
                fprintf(stderr, "[deckmenuhotkey] readEvent Err %d\n", errno);
                return {};
            }
        }
        else
        {
            if (n != sizeof ev)
            {
                errno = EIO;
                return {};
            }
        }
        if (ev.type == EV_KEY)
            setFlags(ev);
        return ev;
    }
    else
    {
        return {};
    }
}

void Keyboard::setFlags(input_event &ev)
{
    // Released
    if (ev.value == 0)
    {
        if (ev.code == KEY_LEFTALT)
            modifiers.left.alt = false;
        if (ev.code == KEY_LEFTCTRL)
            modifiers.left.ctrl = false;
        if (ev.code == KEY_LEFTSHIFT)
            modifiers.left.shift = false;
        if (ev.code == KEY_LEFTMETA)
            modifiers.left.meta = false;
        if (ev.code == KEY_RIGHTALT)
            modifiers.right.alt = false;
        if (ev.code == KEY_RIGHTCTRL)
            modifiers.right.ctrl = false;
        if (ev.code == KEY_RIGHTSHIFT)
            modifiers.right.shift = false;
        if (ev.code == KEY_RIGHTMETA)
            modifiers.right.meta = false;
    }

    // Pressed
    if (ev.value == 1 || ev.value == 2)
    {
        if (ev.code == KEY_LEFTALT)
            modifiers.left.alt = true;
        if (ev.code == KEY_LEFTCTRL)
            modifiers.left.ctrl = true;
        if (ev.code == KEY_LEFTSHIFT)
            modifiers.left.shift = true;
        if (ev.code == KEY_LEFTMETA)
            modifiers.left.meta = true;
        if (ev.code == KEY_RIGHTALT)
            modifiers.right.alt = true;
        if (ev.code == KEY_RIGHTCTRL)
            modifiers.right.ctrl = true;
        if (ev.code == KEY_RIGHTSHIFT)
            modifiers.right.shift = true;
        if (ev.code == KEY_RIGHTMETA)
            modifiers.right.meta = true;
    }
}

void Keyboard::setHotkeys(const HotkeyGroupsEnum hotkey_type, const bool alt, const bool ctrl, const bool shift, const bool meta, const unsigned short ev_key)
{
    hotkeys.at(hotkey_type).modifiers.alt = alt;
    hotkeys.at(hotkey_type).modifiers.ctrl = ctrl;
    hotkeys.at(hotkey_type).modifiers.shift = shift;
    hotkeys.at(hotkey_type).modifiers.meta = meta;
    hotkeys.at(hotkey_type).key = ev_key;
}

void Keyboard::setHotkeys(const HotkeyGroupsEnum hotkey_type, const HotkeyGroup &hotkey)
{
    hotkeys.at(hotkey_type) = hotkey;
}

bool Keyboard::testModifiers(const HotkeyGroupsEnum hotkey_type)
{
    return ((hotkeys.at(hotkey_type).modifiers.alt == (modifiers.left.alt || modifiers.right.alt)) && (hotkeys.at(hotkey_type).modifiers.ctrl == (modifiers.left.ctrl || modifiers.right.ctrl)) && (hotkeys.at(hotkey_type).modifiers.shift == (modifiers.left.shift || modifiers.right.shift)) && (hotkeys.at(hotkey_type).modifiers.meta == (modifiers.left.meta || modifiers.right.meta)));
}

std::optional<HotkeyGroupsEnum> Keyboard::testHotkey()
{
    auto ev = readEvent();
    if (ev.has_value() && ev.value().type == EV_KEY && ev.value().value == 0)
        for (size_t i = 0; i < HotkeyGroupsEnum::NUMBER_OF_HOTKEYS; i++)
            if (testModifiers(static_cast<HotkeyGroupsEnum>(i)) && ev.value().code == hotkeys.at(static_cast<HotkeyGroupsEnum>(i)).key)
                return static_cast<HotkeyGroupsEnum>(i);
    return {};
}

HotkeyGroup Keyboard::getHotkeys(const HotkeyGroupsEnum hotkey_type)
{
    return hotkeys.at(hotkey_type);
}

Keyboards::Keyboards()
{
    reloadKeyboards();
}

void Keyboards::stop()
{
    for (auto &keyboard : keyboards)
        saveKeyboard(keyboard.first, keyboard.second);
}

void Keyboards::saveKeyboard(const std::string &name, Keyboard &keyboard)
{
    parent->preferences->saveKeyboard(name, isKeyboardEnabled(name), {{HotkeyGroupsEnum::STEAM, keyboard.getHotkeys(HotkeyGroupsEnum::STEAM)}, {HotkeyGroupsEnum::QUICKMENU, keyboard.getHotkeys(HotkeyGroupsEnum::QUICKMENU)}});
}

void Keyboards::reloadKeyboards()
{
    reader.reloadDevices();
    keyboards.clear();
    for (const auto &device : reader.getDeviceNames())
    {
        if (std::find(excluded_inputs.begin(), excluded_inputs.end(), device) == excluded_inputs.end())
        {
            keyboards.emplace(device, Keyboard(reader.getEventNumber(device)));
            if (parent->preferences->haveKeyboard(device))
            {
                keyboards.at(device).setHotkeys(HotkeyGroupsEnum::STEAM, parent->preferences->getHotkey(device, HotkeyGroupsEnum::STEAM));
                keyboards.at(device).setHotkeys(HotkeyGroupsEnum::QUICKMENU, parent->preferences->getHotkey(device, HotkeyGroupsEnum::QUICKMENU));
                setKeyboardEnabled(device, parent->preferences->isKeyboardEnabled(device));
            }
        }
    }
}

std::vector<std::string> Keyboards::getKeyboardNames()
{
    std::vector<std::string> keyboard_names;
    for (const auto &keyboard : keyboards)
        keyboard_names.push_back(keyboard.first);
    return keyboard_names;
}

void Keyboards::process()
{
    for (auto &keyboard : keyboards)
    {
        if (keyboard.second.isEnabled())
        {
            std::optional<HotkeyGroupsEnum> hotkey = keyboard.second.testHotkey();
            if (hotkey.has_value())
                parent->bindings[hotkey.value()]();
        }
    }
}

void Keyboards::setKeyboardEnabled(const std::string &name, bool enabled)
{
    if (keyboards.contains(name))
    {
        keyboards[name].setEnabled(enabled);
        saveKeyboard(name, keyboards[name]);
    }
}

bool Keyboards::isKeyboardEnabled(const std::string &name)
{
    return (keyboards.contains(name) ? keyboards[name].isEnabled() : false);
}

void Keyboards::setKeyboardHotkeys(const std::string &name, const HotkeyGroupsEnum hotkey_type, const bool alt, const bool ctrl, const bool shift, const bool meta, const unsigned short ev_key)
{
    if (keyboards.contains(name))
    {
        keyboards[name].setHotkeys(hotkey_type, alt, ctrl, shift, meta, ev_key);
        saveKeyboard(name, keyboards[name]);
    }
}

HotkeyGroup Keyboards::getKeyboardHotkeys(const std::string &name, const HotkeyGroupsEnum hotkey_type)
{
    if (keyboards.contains(name))
        return keyboards[name].getHotkeys(hotkey_type);
    return {};
}