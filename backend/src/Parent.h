#ifndef PARENT_H_GUARD
#define PARENT_H_GUARD

#include <memory>

class Manager;
class Parent
{
protected:
    inline static std::shared_ptr<Manager> parent;

public:
    static void init(std::shared_ptr<Manager> parent);
};

#endif