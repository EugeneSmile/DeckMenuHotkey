#ifndef PREFERENCES_H_GUARD
#define PREFERENCES_H_GUARD

#include <string>
#include <vector>
#include <unordered_map>

#include <toml.hpp>

#include "Parent.h"
#include "Types.h"

class ConfigFile
{
private:
    const std::string config_filename = "deckmenuhotkey.toml";
    std::string config_path;
    std::string config_full_path;

public:
    ConfigFile();
    void setPath(const char *path = NULL);
    const std::string &getPath();
};

class Preferences : public Parent
{
private:
    ConfigFile config_file;
    toml::value config;
    std::unordered_map<HotkeyGroupsEnum, std::string> hotkeys_tags;

public:
    Preferences(/* args */);
    void stop();
    bool isAppEnabled();
    void toggleAppEnabled();
    void toggleAppEnabled(bool state);
    bool haveKeyboard(const std::string &keyboard_name);
    const HotkeyGroup getHotkey(const std::string &keyboard_name, HotkeyGroupsEnum hotkey_type);
    void saveKeyboard(const std::string &keyboard_name, bool enabled, std::vector<std::pair<HotkeyGroupsEnum, HotkeyGroup>> hotkeys);
    bool isKeyboardEnabled(const std::string &keyboard_name);
    void printConfig();
};

#endif