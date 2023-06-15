#ifndef MANAGER_H_GUARD
#define MANAGER_H_GUARD

#include "Types.h"

#include "Keyboards.h"
#include "Controller_XBox.h"

class Manager
{
private:
    Keyboards keyboards;
    Controller_XBox controller;

public:
    Manager(/* args */);
    ~Manager();
    void process();
};

#endif