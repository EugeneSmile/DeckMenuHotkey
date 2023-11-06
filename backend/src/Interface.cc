#include "Interface.h"

#include <iostream>
#include <string>
#include <vector>

#include "Homepage.h"
#include "Keyboards.h"

Interface::Interface(std::shared_ptr<Keyboards> keyboards) : keyboards(keyboards)
{
    // app.loglevel(crow::LogLevel::WARNING);
    CROW_ROUTE(app, "/")
    ([]
     {
        crow::mustache::context ctx;
        return crow::mustache::compile(HOMEPAGE_HTML).render(); });

    if (keyboards)
    {
        CROW_ROUTE(app, "/getKeyboards")
        (std::bind(&Interface::getKeyboards, this));
        CROW_ROUTE(app, "/getActiveKeyboards")
        (std::bind(&Interface::getActiveKeyboards, this));
        CROW_ROUTE(app, "/setKeyboardActive")
        (std::bind(&Interface::setKeyboardActive, this, std::placeholders::_1));
        CROW_ROUTE(app, "/getKeyboardHotkeys")
        (std::bind(&Interface::getKeyboardHotkeys, this, std::placeholders::_1));
        CROW_ROUTE(app, "/setKeyboardHotkey")
        (std::bind(&Interface::setKeyboardHotkey, this, std::placeholders::_1));
        CROW_ROUTE(app, "/logRequest")
        (std::bind(&Interface::logRequest, this, std::placeholders::_1));
    }

    app_future = app.bindaddr("127.0.0.1")
                     .port(10336)
                     .run_async();
    CROW_LOG_INFO << "Interface started!";
}

Interface::~Interface()
{
    app.stop();
    // app_future.wait_for(std::chrono::seconds(1));
    app_future.wait();
}

const std::string Interface::fixString(const std::string &str)
{
    std::string fixed_string = str;
    size_t pos = 0;
    while ((pos = fixed_string.find("%20", pos)) != std::string::npos)
    {
        fixed_string.replace(pos, 3, " ");
        pos++;
    }
    return fixed_string;
}

crow::json::wvalue Interface::getKeyboards()
{
    crow::json::wvalue return_value;
    return_value = keyboards->getKeyboards();
    return return_value;
}

crow::json::wvalue Interface::getActiveKeyboards()
{
    crow::json::wvalue return_value;
    return_value = keyboards->getActiveKeyboards();
    return return_value;
}

crow::response Interface::setKeyboardActive(const crow::request &request)
{
    crow::response res;
    res.write(keyboards->setKeyboardActive(fixString(request.url_params.get("keyboard")), crow::utility::lexical_cast<bool>(request.url_params.get("active"))) ? "OK" : "Error");
    return res;
}

crow::json::wvalue Interface::getKeyboardHotkeys(const crow::request &request)
{
    std::string fixed_name = fixString(request.url_params.get("keyboard"));
    auto hotkey_steam = keyboards->getKeyboard(fixed_name).getHotkeys(SteamHotkeys::STEAM);
    auto hotkey_quickmenu = keyboards->getKeyboard(fixed_name).getHotkeys(SteamHotkeys::QUICKMENU);
    return {
        {"Steam", {{"Meta", hotkey_steam.modifiers.meta ? "True" : "False"}, {"Alt", hotkey_steam.modifiers.alt ? "True" : "False"}, {"Ctrl", hotkey_steam.modifiers.ctrl ? "True" : "False"}, {"Shift", hotkey_steam.modifiers.shift ? "True" : "False"}, {"Key", std::to_string(hotkey_steam.key)}}},
        {"Quickmenu",
         {{"Meta", hotkey_quickmenu.modifiers.meta ? "True" : "False"}, {"Alt", hotkey_quickmenu.modifiers.alt ? "True" : "False"}, {"Ctrl", hotkey_quickmenu.modifiers.ctrl ? "True" : "False"}, {"Shift", hotkey_quickmenu.modifiers.shift ? "True" : "False"}, {"Key", std::to_string(hotkey_quickmenu.key)}}}};
}

crow::response Interface::setKeyboardHotkey(const crow::request &request)
{
    std::string fixed_name = fixString(request.url_params.get("keyboard"));
    if (!fixed_name.empty())
    {
        keyboards->getKeyboard(fixed_name).setHotkey(static_cast<SteamHotkeys>(crow::utility::lexical_cast<int>("type")), crow::utility::lexical_cast<bool>(request.url_params.get("meta")), crow::utility::lexical_cast<bool>(request.url_params.get("alt")), crow::utility::lexical_cast<bool>(request.url_params.get("ctrl")), crow::utility::lexical_cast<bool>(request.url_params.get("shift")), crow::utility::lexical_cast<unsigned short>(request.url_params.get("key")));
    }
    crow::response res;
    res.write("OK");
    return res;
}

crow::response Interface::logRequest(const crow::request &request)
{
    for (auto &key : request.url_params.keys())
    {
        if (key == "info")
            CROW_LOG_INFO << fixString(request.url_params.get("info"));
        if (key == "warning")
            CROW_LOG_WARNING << fixString(request.url_params.get("warning"));
        if (key == "error")
            CROW_LOG_ERROR << fixString(request.url_params.get("error"));
        if (key == "debug")
            CROW_LOG_DEBUG << fixString(request.url_params.get("debug"));
    }
    crow::response res;
    res.write("OK");
    return res;
}
