#include "Interface.h"

#include <iostream>
#include <string>
#include <vector>

#include "Manager.h"
#include "Homepage.h"
#include "StringHelper.h"

Interface::Interface()
{
    app.loglevel(crow::LogLevel::WARNING);

    // CORS issue in Crow library https://github.com/CrowCpp/Crow/issues/538
    /*auto &cors = app.get_middleware<crow::CORSHandler>();

    cors.global()
        .methods(crow::HTTPMethod::OPTIONS, crow::HTTPMethod::POST, crow::HTTPMethod::GET, crow::HTTPMethod::HEAD)
        .origin("*")
        .headers("Access-Control-Allow-Origin", "Access-Control-Allow-Methods", "Access-Control-Allow-Headers");*/

    CROW_ROUTE(app, "/")
    ([]
     {
        crow::mustache::context ctx;
        return crow::mustache::compile(HOMEPAGE_HTML).render(); });

    if (parent->keyboards)
    {
        CROW_ROUTE(app, "/getKeyboards")
        (std::bind(&Interface::getKeyboards, this));
        CROW_ROUTE(app, "/setKeyboards").methods(crow::HTTPMethod::POST, crow::HTTPMethod::OPTIONS)(std::bind(&Interface::setKeyboards, this, std::placeholders::_1));
        CROW_ROUTE(app, "/reloadKeyboards")
        (std::bind(&Interface::reloadKeyboards, this));
        CROW_ROUTE(app, "/logRequest")
        (std::bind(&Interface::logRequest, this, std::placeholders::_1));
        CROW_ROUTE(app, "/getEnabled")
        (std::bind(&Interface::getEnabled, this));
        /*CROW_ROUTE(app, "/setEnabled").methods(crow::HTTPMethod::OPTIONS)([](const crow::request &req)
                                                                          { CROW_LOG_INFO << "Sending OPTIONS response"; return crow::response(crow::status::OK); });*/
        CROW_ROUTE(app, "/setEnabled").methods(crow::HTTPMethod::POST, crow::HTTPMethod::OPTIONS)(std::bind(&Interface::setEnabled, this, std::placeholders::_1));
        CROW_ROUTE(app, "/printConfig")
        (std::bind(&Interface::printConfig, this));
        CROW_ROUTE(app, "/stopBackend")
        (std::bind(&Interface::stopBackend, this));
    }

    app_future = app.bindaddr("127.0.0.1")
                     .port(10336)
                     .run_async();

    app.wait_for_server_start();
    CROW_LOG_INFO << "[deckmenuhotkey] Interface started!";
}

void Interface::stop()
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
    std::vector<crow::json::wvalue> return_value;
    for (const auto &keyboard : parent->keyboards->getKeyboardNames())
    {
        crow::json::wvalue keyboard_object;
        keyboard_object["name"] = keyboard;
        keyboard_object["enabled"] = (parent->keyboards->isKeyboardEnabled(keyboard));
        auto hotkey_steam = parent->keyboards->getKeyboardHotkeys(keyboard, HotkeyGroupsEnum::STEAM);
        auto hotkey_quickmenu = parent->keyboards->getKeyboardHotkeys(keyboard, HotkeyGroupsEnum::QUICKMENU);
        keyboard_object["hotkeys"] = {
            {"steam", {{"meta", hotkey_steam.modifiers.meta}, {"alt", hotkey_steam.modifiers.alt}, {"ctrl", hotkey_steam.modifiers.ctrl}, {"shift", hotkey_steam.modifiers.shift}, {"key", hotkey_steam.key}}},
            {"quickmenu",
             {{"meta", hotkey_quickmenu.modifiers.meta}, {"alt", hotkey_quickmenu.modifiers.alt}, {"ctrl", hotkey_quickmenu.modifiers.ctrl}, {"shift", hotkey_quickmenu.modifiers.shift}, {"key", hotkey_quickmenu.key}}}};
        return_value.push_back(keyboard_object);
    }
    return return_value;
}

crow::response Interface::setKeyboards(const crow::request &request)
{
    auto req_json = crow::json::load(request.body);
    if (!req_json)
        return crow::response(400);

    if (req_json.t() == crow::json::type::List)
    {
        for (auto keyboard_json : req_json.lo())
        {
            parent->keyboards->setKeyboardEnabled(keyboard_json["name"].s(), keyboard_json["enabled"].b());
            parent->keyboards->setKeyboardHotkeys(keyboard_json["name"].s(), HotkeyGroupsEnum::STEAM, keyboard_json["hotkeys"]["steam"]["alt"].b(), keyboard_json["hotkeys"]["steam"]["ctrl"].b(), keyboard_json["hotkeys"]["steam"]["shift"].b(), keyboard_json["hotkeys"]["steam"]["meta"].b(), static_cast<unsigned short>(keyboard_json["hotkeys"]["steam"]["key"].u()));
            parent->keyboards->setKeyboardHotkeys(keyboard_json["name"].s(), HotkeyGroupsEnum::QUICKMENU, keyboard_json["hotkeys"]["quickmenu"]["alt"].b(), keyboard_json["hotkeys"]["quickmenu"]["ctrl"].b(), keyboard_json["hotkeys"]["quickmenu"]["shift"].b(), keyboard_json["hotkeys"]["quickmenu"]["meta"].b(), static_cast<unsigned short>(keyboard_json["hotkeys"]["quickmenu"]["key"].u()));
        }
    }
    return crow::response(200);
}

crow::response Interface::reloadKeyboards()
{
    parent->keyboards->reloadKeyboards();
    return crow::response(200);
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

crow::json::wvalue Interface::getEnabled()
{
    return parent->preferences->isAppEnabled();
}

crow::response Interface::setEnabled(const crow::request &request)
{
    parent->preferences->toggleAppEnabled(crow::json::load(request.body).b());
    if (parent->controller->isCreated() && !crow::json::load(request.body).b())
        parent->controller->destroy();
    else if (!parent->controller->isCreated() && crow::json::load(request.body).b())
        parent->controller->recreate();
    return crow::response(200);
}

crow::response Interface::printConfig()
{
    parent->preferences->printConfig();
    return crow::response(200);
}

crow::response Interface::stopBackend()
{
    parent->stop();
    return crow::response(200);
}
