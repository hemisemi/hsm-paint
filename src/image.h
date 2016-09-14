#pragma once

#include "paintdevice.h"
#include <hsm/bitmap.h>
#include <hsm/types.h>
#include "graphiccontext.h"
#include <hsm/rect.h>
#include <hsm/color.h>

#include <map>

namespace hsm{

class image : public bitmap, public paintdevice{
    friend class bitmap_paintengine;
public:
    image();
    image(const bitmap & bitmap);
    image(uint8_t *data, size_t width, size_t height, pixel_format format, bool copy = true);
    image(uint8_t *data, size_t width, size_t height, pixel_format format, size_t stride, bool copy = true);
    image(const std::string & filename);
    image(size_t width, size_t height, pixel_format format = pixel_format::argb);
    image(size_t width, size_t height, pixel_format format, size_t stride);

    size_t width() const;
    size_t height() const;

    static size_t prefered_stride(bitmap::pixel_format, size_t width);
    hsm::paintengine *paint_engine() const;
};

}
