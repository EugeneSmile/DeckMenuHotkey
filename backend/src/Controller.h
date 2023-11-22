#ifndef CONTROLLER_H_GUARD
#define CONTROLLER_H_GUARD

#include <string>
#include <vector>
#include <cstdint>

#include <linux/uinput.h>

#include "Parent.h"
#include "InputDevice.h"

class Controller : public InputDevice, public Parent
{
private:
protected:
    bool created{false};
    std::vector<int> keys{-1};
    uinput_user_dev udev;

    void init();

public:
    Controller(uint16_t vid, uint16_t pid, uint16_t version, const std::string &name);
    void create();
    void destroy();
    void recreate();
    bool isCreated();
    void stop();
    void sendSteam();
    void sendQuickmenu();
};

#endif