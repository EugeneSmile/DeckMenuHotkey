#include "Controller.h"

#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <thread>

Controller::Controller(uint16_t vid, uint16_t pid, uint16_t version, const std::string &name)
{
    std::cout << "Creating controller base for controller " << name << std::endl;
    std::memset(&udev, 0, sizeof(udev));
    udev.id.bustype = BUS_USB;
    udev.id.vendor = vid;  /* sample vendor */
    udev.id.product = pid; /* sample product */
    udev.id.version = version;
    strcpy(udev.name, name.c_str());

    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
}

Controller::~Controller()
{
    std::cout << "Removing controller base for controller " << udev.name << std::endl;
    ioctl(fd, UI_DEV_DESTROY);
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

    ioctl(fd, UI_DEV_CREATE);

    std::this_thread::sleep_for(std::chrono::seconds(1));
}
