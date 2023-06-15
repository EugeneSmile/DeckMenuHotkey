#include "Controller_XBox.h"

#include <string>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <chrono>

Controller_XBox::Controller_XBox() : Controller(0x045e, 0x028e, 0x110, "XBox 360 Controller (Virtual)")
{
    std::cout << "Initializing controller " << udev.name << std::endl;
    keys = {BTN_SOUTH, BTN_EAST, BTN_NORTH, BTN_WEST, BTN_SELECT, BTN_MODE, BTN_START, BTN_TL, BTN_TR, BTN_THUMBL, BTN_THUMBR, -1};

    init();
}

Controller_XBox::~Controller_XBox()
{
}

void Controller_XBox::sendSteam()
{
    emit(EV_KEY, BTN_MODE, 1);
    emit(EV_SYN, SYN_REPORT, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    emit(EV_KEY, BTN_MODE, 0);
    emit(EV_SYN, SYN_REPORT, 0);
}

void Controller_XBox::sendQuickmenu()
{
    emit(EV_KEY, BTN_MODE, 1);
    emit(EV_SYN, SYN_REPORT, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    emit(EV_KEY, BTN_SOUTH, 1);
    emit(EV_SYN, SYN_REPORT, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    emit(EV_KEY, BTN_MODE, 0);
    emit(EV_SYN, SYN_REPORT, 0);
    emit(EV_KEY, BTN_SOUTH, 0);
    emit(EV_SYN, SYN_REPORT, 0);
}
