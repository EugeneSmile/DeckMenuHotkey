#include "Preferences.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstdlib>

ConfigFile::ConfigFile()
{
    setPath();
}
void ConfigFile::setPath(const char *path)
{
    if (path != NULL)
        config_path = path;
    else if (config_path.empty())
        config_path = {};
    config_full_path = config_path + (config_path.empty() ? "" : "/") + config_filename;
}
const std::string &ConfigFile::getPath()
{
    return config_full_path;
}

Preferences::Preferences(/* args */) : hotkeys_tags({{HotkeyGroupsEnum::STEAM, "Steam"},
                                                     {HotkeyGroupsEnum::QUICKMENU, "Quickmenu"}})
{
    config_file.setPath(std::getenv("DECKY_PLUGIN_SETTINGS_DIR"));
    std::cout << "[deckmenuhotkey] Using config at path: " << config_file.getPath() << std::endl;
    if (!std::filesystem::exists(config_file.getPath()))
    {
        std::ofstream config_file_stream(config_file.getPath());
        config_file_stream << "\n";
        config_file_stream.close();
    }
    config = toml::parse(config_file.getPath());
    if (!config.contains("General"))
        config["General"] = {};
    auto &general = toml::find<toml::table>(config, "General");
    if (!general.contains("enabled"))
        general.insert_or_assign("enabled", true);
    if (!config.contains("Keyboards"))
        config["Keyboards"] = {};
}

void Preferences::stop()
{
    std::ofstream config_file_stream(config_file.getPath());
    config_file_stream << config;
    config_file_stream.close();
}

bool Preferences::isAppEnabled()
{
    return toml::find<toml::boolean>(config, "General", "enabled");
}

void Preferences::toggleAppEnabled()
{
    toml::find<toml::boolean>(config, "General", "enabled") = !toml::find<toml::boolean>(config, "General", "enabled");
}

void Preferences::toggleAppEnabled(bool state)
{
    toml::find<toml::boolean>(config, "General", "enabled") = state;
}

bool Preferences::haveKeyboard(const std::string &keyboard_name)
{
    return toml::find<toml::value>(config, "Keyboards").contains(keyboard_name);
}

const HotkeyGroup Preferences::getHotkey(const std::string &keyboard_name, HotkeyGroupsEnum hotkey_type)
{
    HotkeyGroup return_hotkey;

    return_hotkey.modifiers.alt = toml::find<toml::boolean>(config, "Keyboards", keyboard_name, hotkeys_tags[hotkey_type], "alt");
    return_hotkey.modifiers.ctrl = toml::find<toml::boolean>(config, "Keyboards", keyboard_name, hotkeys_tags[hotkey_type], "ctrl");
    return_hotkey.modifiers.shift = toml::find<toml::boolean>(config, "Keyboards", keyboard_name, hotkeys_tags[hotkey_type], "shift");
    return_hotkey.modifiers.meta = toml::find<toml::boolean>(config, "Keyboards", keyboard_name, hotkeys_tags[hotkey_type], "meta");
    return_hotkey.key = toml::find<toml::integer>(config, "Keyboards", keyboard_name, hotkeys_tags[hotkey_type], "key");

    return return_hotkey;
}

void Preferences::saveKeyboard(const std::string &keyboard_name, bool enabled, std::vector<std::pair<HotkeyGroupsEnum, HotkeyGroup>> hotkeys)
{
    auto &toml_keyboards = toml::find<toml::table>(config, "Keyboards");
    toml_keyboards[keyboard_name] = {{"enabled", enabled}};
    for (auto &hotkey : hotkeys)
    {
        toml_keyboards[keyboard_name][hotkeys_tags[hotkey.first]] = {
            {"alt", hotkey.second.modifiers.alt},
            {"ctrl", hotkey.second.modifiers.ctrl},
            {"shift", hotkey.second.modifiers.shift},
            {"meta", hotkey.second.modifiers.meta},
            {"key", hotkey.second.key}};
    }
}

bool Preferences::isKeyboardEnabled(const std::string &keyboard_name)
{
    return (toml::find<toml::table>(config, "Keyboards").contains(keyboard_name) ? toml::find<toml::boolean>(config, "Keyboards", keyboard_name, "enabled") : false);
}

void Preferences::printConfig()
{
    std::cout << config << std::endl;
}
