#pragma once

#include <stddef.h>

namespace hsm{

class paintengine;

class paintdevice{
public:
    paintdevice();
    virtual ~paintdevice();

    virtual paintengine *paint_engine() const = 0;

    virtual size_t width() const = 0;
    virtual size_t height() const = 0;
};

}
