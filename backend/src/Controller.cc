#include "Controller.h"

#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <thread>
#include <chrono>

Controller::Controller(uint16_t vid, uint16_t pid, uint16_t version, const std::string &name)
{
    std::cout << "[deckmenuhotkey] Creating controller base for controller " << name << std::endl;
    std::memset(&udev, 0, sizeof(udev));
    udev.id.bustype = BUS_USB;
    udev.id.vendor = vid;
    udev.id.product = pid;
    udev.id.version = version;
    strcpy(udev.name, name.c_str());

    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
}

void Controller::stop()
{
    std::cout << "[deckmenuhotkey] Removing controller base for controller " << udev.name << std::endl;
    destroy();
    close(fd);
}

void Controller::init()
{
    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    for (int i = 0; keys[i] >= 0; i++)
    {
        ioctl(fd, UI_SET_KEYBIT, keys[i]);
    }

    ioctl(fd, UI_SET_PHYS, "");

    ssize_t res = write(fd, &udev, sizeof(udev));
}

void Controller::create()
{
    ioctl(fd, UI_DEV_CREATE);
    created = true;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Controller::destroy()
{
    ioctl(fd, UI_DEV_DESTROY);
    created = false;
}

void Controller::recreate()
{
    init();
    create();
}

bool Controller::isCreated()
{
    return created;
}

void Controller::sendSteam()
{
    emit(EV_KEY, BTN_MODE, 1);
    emit(EV_SYN, SYN_REPORT, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    emit(EV_KEY, BTN_MODE, 0);
    emit(EV_SYN, SYN_REPORT, 0);
}

void Controller::sendQuickmenu()
{
    emit(EV_KEY, BTN_MODE, 1);
    emit(EV_SYN, SYN_REPORT, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    emit(EV_KEY, BTN_SOUTH, 1);
    emit(EV_SYN, SYN_REPORT, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    emit(EV_KEY, BTN_MODE, 0);
    emit(EV_SYN, SYN_REPORT, 0);
    emit(EV_KEY, BTN_SOUTH, 0);
    emit(EV_SYN, SYN_REPORT, 0);
}
