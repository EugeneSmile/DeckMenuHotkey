#include "Manager.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

std::shared_ptr<Manager> Manager::create()
{
    std::shared_ptr<Manager> manager = std::make_shared<Manager>();
    manager->init();
    return manager;
}

void Manager::init()
{
    Parent::init(shared_from_this());

    preferences = std::make_shared<Preferences>();
    controller = std::make_shared<Controller_Custom>();
    keyboards = std::make_shared<Keyboards>();
    interface = std::make_shared<Interface>();

    if (preferences->isAppEnabled())
        controller->create();

    bindings = {
        {HotkeyGroupsEnum::STEAM, std::bind(&Controller_Custom::sendSteam, controller)},
        {HotkeyGroupsEnum::QUICKMENU, std::bind(&Controller_Custom::sendQuickmenu, controller)}};

    Manager::run = true;
}

std::shared_ptr<Manager> Manager::getPointer()
{
    return shared_from_this();
}

void Manager::process()
{
    while (run)
    {
        if (preferences->isAppEnabled())
        {
            keyboards->process();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
    controller->stop();
    keyboards->stop();
    interface->stop();
    preferences->stop();
}

void Manager::stop()
{
    Manager::run = false;
}