#ifndef RAWFONT_FNT_H
#define RAWFONT_FNT_H

#include <map>
#include <hsm/rect.h>
#include <hsm/types.h>
#include "rawfont.h"

namespace hsm{

class bitmap;

class rawfont_fnt : public rawfont{
public:
    struct glyph{
        hsm::rect rect;
        glyphmetrics metrics;
    };

    rawfont_fnt(hsm::bitmap *bmp, std::map<uint32_t, glyph> glyphs);
    ~rawfont_fnt();

    bool matchStyle(font::Style) const;
    glyphmetrics metrics(int pixel_size, uint32_t charcode) const;
    int height(int pixel_size) const;
    int ascender(int pixel_size) const;

    const glyph *get_glyph(uint32_t) const;
    hsm::bitmap *bitmap() const;

    static rawfont_fnt *fromBitmap(hsm::bitmap *, int cw, int ch);

private:
    hsm::bitmap *_bmp;
    mutable std::map<uint32_t, glyph> _map;
    int _height;
};

}

#endif // RAWFONT_FNT_H
