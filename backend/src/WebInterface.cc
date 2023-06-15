#include "WebInterface.h"

#include <iostream>

WebInterface::WebInterface(/* args */)
{
    get_input_function = std::make_shared<std::function<void()>>([]()
                                                                 { return "hello"; });
    CROW_ROUTE(app, "/getInputs")
    ([]()
     { return "hello"; });
    app_future = app.bindaddr("127.0.0.1")
                     .port(10336)
                     .run_async();
    std::cout << "WebInterface started!" << std::endl;
}

WebInterface::~WebInterface()
{
}

void WebInterface::stop()
{
    std::cout << "Stopping WebInterface" << std::endl;
    app.stop();
    std::cout << "Stopping 1" << std::endl;
    std::cout << app_future.valid() << std::endl;
}

void WebInterface::bindGetInputFunction(std::shared_ptr<std::function<void()>> function)
{
    get_input_function = function;
}