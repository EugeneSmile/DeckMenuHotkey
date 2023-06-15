#ifndef INPUTDEVICE_H_GUARD
#define INPUTDEVICE_H_GUARD

class InputDevice
{
protected:
    int fd;

public:
    InputDevice(/* args */);
    ~InputDevice();
    void emit(int type, int code, int val);
};

#endif