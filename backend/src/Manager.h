#ifndef MANAGER_H_GUARD
#define MANAGER_H_GUARD

#include <memory>

#include "Types.h"

#include "Interface.h"
#include "Keyboards.h"
#include "Controller_XBox.h"

class Manager
{
private:
    std::shared_ptr<Keyboards> keyboards;
    std::shared_ptr<Controller_XBox> controller;
    std::shared_ptr<Interface> interface;

public:
    Manager(/* args */);
    ~Manager();
    void process();
};

#endif