#include "Manager.h"

#include <iostream>
#include <chrono>
#include <thread>

Manager::Manager(/* args */)
{
    controller = std::make_shared<Controller_XBox>();
    keyboards = std::make_shared<Keyboards>();
    interface = std::make_shared<Interface>(keyboards);
}

Manager::~Manager()
{
}

void Manager::process()
{
    if (keyboards->process(SteamHotkeys::STEAM))
        std::cout << "Steam hotkey" << std::endl;
    if (keyboards->process(SteamHotkeys::QUICKMENU))
        std::cout << "Quickmenu hotkey" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
}
