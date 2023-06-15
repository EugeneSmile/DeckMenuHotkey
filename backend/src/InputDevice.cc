#include "InputDevice.h"

#include <linux/input.h>
#include <unistd.h>

InputDevice::InputDevice(/* args */)
{
}

InputDevice::~InputDevice()
{
}

void InputDevice::emit(int type, int code, int val)
{
    struct input_event ie;

    ie.type = type;
    ie.code = code;
    ie.value = val;
    /* timestamp values below are ignored */
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;

    write(fd, &ie, sizeof(ie));
}