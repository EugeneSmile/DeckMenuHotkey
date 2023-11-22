#ifndef INTERFACE_H_GUARD
#define INTERFACE_H_GUARD

#include <string>
#include <memory>
#include <functional>
#include <future>

#include "crow.h"
#include "crow/middlewares/cors.h"

#include "Parent.h"

class Interface : public Parent
{
private:
    crow::App<crow::CORSHandler> app;
    std::future<void> app_future;
    const std::string fixString(const std::string &str);
    crow::json::wvalue getKeyboards();
    crow::response setKeyboards(const crow::request &request);
    crow::response logRequest(const crow::request &request);
    crow::json::wvalue getEnabled();
    crow::response setEnabled(const crow::request &request);
    crow::response printConfig();
    crow::response stopBackend();
    crow::response reloadKeyboards();

public:
    Interface();
    void stop();
};

#endif