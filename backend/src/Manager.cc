#include "Manager.h"

#include <iostream>
#include <chrono>
#include <thread>

Manager::Manager(/* args */)
{
}

Manager::~Manager()
{
}

void Manager::process()
{
    if (keyboards.process(SteamHotKeys::STEAM))
        std::cout << "Steam hotkey" << std::endl;
    if (keyboards.process(SteamHotKeys::QUICKMENU))
        std::cout << "Quickmenu hotkey" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
}