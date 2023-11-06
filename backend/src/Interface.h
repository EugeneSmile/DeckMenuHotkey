#ifndef INTERFACE_H_GUARD
#define INTERFACE_H_GUARD

#include <string>
#include <memory>
#include <functional>
#include <future>

#include "crow.h"

class Keyboards;
class Interface
{
private:
    crow::SimpleApp app;
    std::future<void> app_future;
    std::shared_ptr<Keyboards> keyboards;
    const std::string fixString(const std::string &str);
    crow::json::wvalue getKeyboards();
    crow::json::wvalue getActiveKeyboards();
    crow::response setKeyboardActive(const crow::request &request);
    crow::json::wvalue getKeyboardHotkeys(const crow::request &request);
    crow::response setKeyboardHotkey(const crow::request &request);
    crow::response logRequest(const crow::request &request);

public:
    Interface(std::shared_ptr<Keyboards> keyboards = nullptr);
    ~Interface();
};

#endif