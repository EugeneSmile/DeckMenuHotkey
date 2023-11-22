#ifndef MANAGER_H_GUARD
#define MANAGER_H_GUARD

#include <memory>
#include <unordered_map>
#include <functional>

#include "Types.h"
#include "Parent.h"
#include "Interface.h"
#include "Keyboards.h"
#include "Controller_Custom.h"
#include "Preferences.h"

class Interface;
class Keyboards;
class Controller_Custom;
class Preferences;

class Manager : public std::enable_shared_from_this<Manager>
{
private:
    inline static bool run = false;

public:
    Manager() = default;
    void process();
    void init();
    [[nodiscard]] static std::shared_ptr<Manager> create();
    std::shared_ptr<Manager> getPointer();
    void stop();

    std::shared_ptr<Keyboards> keyboards;
    std::shared_ptr<Controller_Custom> controller;
    std::shared_ptr<Interface> interface;
    std::shared_ptr<Preferences> preferences;

    std::unordered_map<HotkeyGroupsEnum, std::function<void(void)>> bindings;
};

#endif