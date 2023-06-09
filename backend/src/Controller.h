#ifndef CONTROLLER_H_GUARD
#define CONTROLLER_H_GUARD

#include <string>
#include <vector>
#include <cstdint>

#include <linux/uinput.h>

class Controller
{
private:
protected:
    int fd;
    std::vector<int> keys{-1};
    uinput_user_dev udev;

    void init();

public:
    Controller(uint16_t vid, uint16_t pid, uint16_t version, const std::string &name);
    ~Controller();
    void emit(int type, int code, int val);
};

#endif