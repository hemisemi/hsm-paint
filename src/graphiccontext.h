#pragma once

#include <hsm/types.h>
#include <stddef.h>

namespace hsm{

class GraphicContext{
public:
    virtual ~GraphicContext();

    virtual size_t getTextureId(const uint8_t *data, char internal_format, int width, int height) = 0;
    virtual void bindTexture(size_t id) = 0;
    virtual void releaseTexture() = 0;
    virtual void refreshTexture(size_t id, const uint8_t *data, char internal_format, int width, int height) = 0;
    virtual void destroyTexture(size_t id) = 0;

    virtual void make_current();
    virtual void release();

    static GraphicContext *current();

protected:
    static GraphicContext *_current;
};

}
