#ifndef FONTMETRICS_H
#define FONTMETRICS_H

#include "font.h"
#include <hsm/rectf.h>
#include <hsm/types.h>

namespace hsm{

struct glyphmetrics{
    float width;
    float height;
    float bearing_x;
    float bearing_y;
    float advance;
};

class fontmetrics{
public:
    fontmetrics();
    fontmetrics(const font & font);

    float width(uint32_t charcode) const;
    float advance(uint32_t charcode) const;
    float height() const;

    glyphmetrics glyphMetrics(uint32_t charcode) const;

    float ascender() const;

    rectf box(const std::string & str, float width = 0.0f) const;

private:
    font _font;
};

}

#endif // FONTMETRICS_H
