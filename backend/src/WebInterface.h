#ifndef WEBINTERFACE_H_GUARD
#define WEBINTERFACE_H_GUARD

#include <memory>
#include <functional>
#include <future>

#include "crow.h"

class WebInterface
{
private:
    crow::SimpleApp app;
    std::future<void> app_future;
    std::shared_ptr<std::function<void()>> get_input_function;

public:
    WebInterface(/* args */);
    ~WebInterface();
    void bindGetInputFunction(std::shared_ptr<std::function<void()>> function);
    void stop();
};

#endif