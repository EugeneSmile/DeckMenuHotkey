#include "Controller_XBox.h"

#include <string>
#include <iostream>
#include <unistd.h>

Controller_XBox::Controller_XBox() : Controller(0x045e, 0x028e, 0x110, "XBox 360 Controller (Virtual)")
{
    std::cout << "Initializing controller " << udev.name << std::endl;
    keys = {BTN_SOUTH, BTN_EAST, BTN_NORTH, BTN_WEST, BTN_SELECT, BTN_MODE, BTN_START, BTN_TL, BTN_TR, BTN_THUMBL, BTN_THUMBR, -1};

    init();
}

Controller_XBox::~Controller_XBox()
{
}
