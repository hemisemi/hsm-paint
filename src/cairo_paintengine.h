#ifndef CAIRO_PAINTENGINE_H
#define CAIRO_PAINTENGINE_H

#include "paintengine.h"
#include <hsm/bitmap.h>
#include <cairo/cairo.h>

namespace hsm{

class cairo_paintengine : public paintengine{
public:
    cairo_paintengine(uint8_t *data, size_t width, size_t height, bitmap::pixel_format format, size_t stride);
    cairo_paintengine(cairo_surface_t *);
    ~cairo_paintengine();

    static cairo_format_t cairo_format(bitmap::pixel_format f);
    static size_t prefered_stride(bitmap::pixel_format f, size_t width);
    static bool is_valid_format(bitmap::pixel_format f);

    cairo_surface_t *surface() const;

    void begin(paintdevice *pdev);
    void end();

    void clip(const rect &region);

    void draw_points(const point *points, int n);
    void draw_points(const pointf *points, int n);
    void draw_rects(const rect *rects, int n);
    void draw_rects(const rectf *rects, int n);
    void draw_bitmap(const rect &target, const bitmap &bitmap, const rect &source);

protected:
    void matrix_changed(const hsm::matrix3x3 & m);

private:
    void set_pen_as_source();
    void set_brush_as_source();

    cairo_t *_cr;
    cairo_surface_t *_surface;
};

}

#endif // CAIRO_PAINTENGINE_H
