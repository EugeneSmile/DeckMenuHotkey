#include "Parent.h"

#include "Manager.h"

void Parent::init(std::shared_ptr<Manager> parent)
{
    Parent::parent = parent;
}