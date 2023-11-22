#include "Controller_Custom.h"

#include <string>
#include <iostream>
#include <unistd.h>

Controller_Custom::Controller_Custom() : Controller(0xF055, 0x0754, 0x0, "DeckMenuHotkey Virtual Controller")
{
    std::cout << "[deckmenuhotkey] Initializing controller " << udev.name << std::endl;
    keys = {BTN_SOUTH, BTN_EAST, BTN_NORTH, BTN_WEST, BTN_SELECT, BTN_MODE, BTN_START, BTN_TL, BTN_TR, BTN_THUMBL, BTN_THUMBR, -1};

    init();
}
