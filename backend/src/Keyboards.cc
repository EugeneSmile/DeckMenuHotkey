#include "Keyboards.h"

#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

Keyboard::Keyboard(int event_number) : event_number(event_number)
{
}

Keyboard::~Keyboard()
{
    closeDevice();
}

bool Keyboard::openDevice()
{
    std::string event_path = "/dev/input/event" + std::to_string(event_number);
    fd = open(event_path.c_str(), O_RDONLY | O_NONBLOCK);
    if (fd == -1)
    {
        error_number = errno;
        fprintf(stderr, "Cannot open %s: %s.\n", event_path, strerror(errno));
        return false;
    }
    is_open = true;
    return true;
}

void Keyboard::closeDevice()
{
    if (is_open)
        close(fd);
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
                fprintf(stderr, "Err %d\n", errno);
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
        if (ev.code == KEY_LEFTMETA)
            modifiers.left.meta = false;
        if (ev.code == KEY_LEFTALT)
            modifiers.left.alt = false;
        if (ev.code == KEY_LEFTCTRL)
            modifiers.left.ctrl = false;
        if (ev.code == KEY_LEFTSHIFT)
            modifiers.left.shift = false;
        if (ev.code == KEY_RIGHTMETA)
            modifiers.right.meta = false;
        if (ev.code == KEY_RIGHTALT)
            modifiers.right.alt = false;
        if (ev.code == KEY_RIGHTCTRL)
            modifiers.right.ctrl = false;
        if (ev.code == KEY_RIGHTSHIFT)
            modifiers.right.shift = false;
    }

    // Pressed
    if (ev.value == 1)
    {
        if (ev.code == KEY_LEFTMETA)
            modifiers.left.meta = true;
        if (ev.code == KEY_LEFTALT)
            modifiers.left.alt = true;
        if (ev.code == KEY_LEFTCTRL)
            modifiers.left.ctrl = true;
        if (ev.code == KEY_LEFTSHIFT)
            modifiers.left.shift = true;
        if (ev.code == KEY_RIGHTMETA)
            modifiers.right.meta = true;
        if (ev.code == KEY_RIGHTALT)
            modifiers.right.alt = true;
        if (ev.code == KEY_RIGHTCTRL)
            modifiers.right.ctrl = true;
        if (ev.code == KEY_RIGHTSHIFT)
            modifiers.right.shift = true;
    }
}

void Keyboard::setHotkey(const SteamHotKeys hotkey_type, const bool meta, const bool alt, const bool ctrl, const bool shift, const unsigned short ev_key)
{
    hotkeys.at(hotkey_type).modifiers.meta = meta;
    hotkeys.at(hotkey_type).modifiers.alt = alt;
    hotkeys.at(hotkey_type).modifiers.ctrl = ctrl;
    hotkeys.at(hotkey_type).modifiers.shift = shift;
    hotkeys.at(hotkey_type).key = ev_key;
}

bool Keyboard::testModifiers(const SteamHotKeys hotkey_type)
{
    return (hotkeys.at(hotkey_type).modifiers.meta == modifiers.left.meta || hotkeys.at(hotkey_type).modifiers.meta == modifiers.right.meta) && (hotkeys.at(hotkey_type).modifiers.alt == modifiers.left.alt || hotkeys.at(hotkey_type).modifiers.alt == modifiers.right.alt) && (hotkeys.at(hotkey_type).modifiers.ctrl == modifiers.left.ctrl || hotkeys.at(hotkey_type).modifiers.ctrl == modifiers.right.ctrl) && (hotkeys.at(hotkey_type).modifiers.shift == modifiers.left.shift || hotkeys.at(hotkey_type).modifiers.shift == modifiers.right.shift);
}

bool Keyboard::isHotkeyPressed(const SteamHotKeys hotkey_type)
{
    auto ev = readEvent();
    if (ev.has_value())
    {
        return (ev.value().type == EV_KEY && testModifiers(hotkey_type) && ev.value().value == 0 && ev.value().code == hotkeys.at(hotkey_type).key);
    }
    return false;
}

Keyboards::Keyboards(/* args */)
{
    for (const auto &keyboard : reader.getKeyboards())
    {
        keyboards.emplace(keyboard, Keyboard(reader.getEventNumber(keyboard)));
    }
    if (keyboards.find(default_keyboard_name) != keyboards.end())
    {
        active_keyboards.push_back(default_keyboard_name);
        keyboards.at(default_keyboard_name).setHotkey(SteamHotKeys::STEAM, true, false, false, false, KEY_HOMEPAGE);
        keyboards.at(default_keyboard_name).setHotkey(SteamHotKeys::QUICKMENU, true, false, false, false, KEY_TAB);
        keyboards.at(default_keyboard_name).openDevice();
    }
}

Keyboards::~Keyboards()
{
}

std::vector<std::string> &Keyboards::getKeyboards()
{
    return reader.getKeyboards();
}

Keyboard &Keyboards::getKeyboard(const std::string &name)
{
    return keyboards[name];
}

bool Keyboards::process(const SteamHotKeys hotkey_type)
{
    bool return_value = false;
    for (const auto &keyboard : active_keyboards)
    {
        return_value |= keyboards.at(keyboard).isHotkeyPressed(hotkey_type);
    }
    return return_value;
}