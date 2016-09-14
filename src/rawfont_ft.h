#ifndef RAWFONT_FT_H
#define RAWFONT_FT_H

#include "string.h"
#include "rawfont.h"
#include "fontmetrics.h"
#include <map>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

namespace hsm{

class rawfont_ft : public rawfont{
public:
    rawfont_ft(FT_Face face, const uint8_t *data);
    ~rawfont_ft();

    struct bitmap{
        uint8_t *data;
        int width;
        int height;
    };

    struct glyph{
        rawfont_ft::bitmap bitmap;
        glyphmetrics metrics;
    };

    bool matchStyle(font::Style) const;
    glyphmetrics metrics(int pixel_size, uint32_t charcode) const;
    int height(int pixel_size) const;
    int ascender(int pixel_size) const;
    std::string family() const;

    const rawfont_ft::glyph *get_glyph(int pixel_size, uint32_t) const;

private:
    const uint8_t *_data;
    std::string _family;
    font::Style _style;
    FT_Face _face;
    mutable std::map<int, std::map<uint32_t, glyph>> _charmap;
};

}

#endif // RAWFONT_FT_H
