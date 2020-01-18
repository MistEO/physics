#pragma once

#include <vector>

#include "define.h"
#include "object.h"

class Time;

class Space {

public:
    virtual void push_object(Object* object);
    virtual void tick(double time = 1.0);

protected:
    friend class Time;
    std::vector<Object*> objects;

private:
    virtual void tick_object(Object* object, double time);
};