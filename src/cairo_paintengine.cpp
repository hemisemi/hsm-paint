#include "cairo_paintengine.h"
#include "cairo_compatible_paintdevice.h"
#include <hsm/point.h>
#include <hsm/pointf.h>
#include <hsm/rect.h>
#include <hsm/rectf.h>
#include <iostream>

namespace hsm{

cairo_paintengine::cairo_paintengine(uint8_t *data, size_t width, size_t height, bitmap::pixel_format format, size_t stride){
    cairo_format_t cformat = cairo_format(format);
    _surface = cairo_image_surface_create_for_data(data, cformat, width, height, stride);
    if(cairo_surface_status(_surface) == CAIRO_STATUS_INVALID_STRIDE){
        std::cerr << "hsm::cairo_paintengine: invalid stride " << stride << std::endl;
        std::cerr << "valid stride would be " << cairo_format_stride_for_width(cformat, width) << std::endl;
        return;
    }
    if(data != cairo_image_surface_get_data(_surface)){
        std::cerr << "hsm::cairo_paintengine: invalid data" << std::endl;
        return;
    }
}

cairo_paintengine::cairo_paintengine(cairo_surface_t *s){
    _surface = s;
}

cairo_paintengine::~cairo_paintengine(){
    end();
    if(_surface != nullptr){
        cairo_surface_finish(_surface);
        cairo_surface_destroy(_surface);
    }
}

size_t cairo_paintengine::prefered_stride(bitmap::pixel_format f, size_t width){
    return cairo_format_stride_for_width(cairo_format(f), width);
}

cairo_format_t cairo_paintengine::cairo_format(bitmap::pixel_format f){
    switch(f){
    case bitmap::pixel_format::gray:
        return CAIRO_FORMAT_A8;
    case bitmap::pixel_format::rgb:
        return CAIRO_FORMAT_RGB24;
    case bitmap::pixel_format::argb:
        return CAIRO_FORMAT_ARGB32;
    default:
        break;
    }
    return CAIRO_FORMAT_INVALID;
}

bool cairo_paintengine::is_valid_format(bitmap::pixel_format f){
    return cairo_format(f) != CAIRO_FORMAT_INVALID;
}

cairo_surface_t *cairo_paintengine::surface() const{
    if(_surface != nullptr)
        return _surface;
    if(!device())
        return nullptr;
    cairo_compatible_paintdevice *cdevice = dynamic_cast<cairo_compatible_paintdevice*>(device());
    if(!cdevice){
        std::cerr << "cairo_paintengine::surface() : Not a cairo compatible paint device." << std::endl;
        return nullptr;
    }
    return cdevice->cairo_surface();
}

void cairo_paintengine::begin(paintdevice *pdev){
    paintengine::begin(pdev);
    cairo_surface_t *s = surface();
    if(s == nullptr){
        paintengine::end();
        std::cerr << "cairo_paintengine::begin() : Cannot paint on this device." << std::endl;
        return;
    }
    _cr = cairo_create(s);
}

void cairo_paintengine::end(){
    cairo_destroy(_cr);
    _cr = 0;
}

void cairo_paintengine::clip(const hsm::rect &){
    //
}

void cairo_paintengine::set_pen_as_source(){
    cairo_set_source_rgba(_cr, pen().color().redf(), pen().color().greenf(), pen().color().bluef(), pen().color().alphaf());
}

void cairo_paintengine::set_brush_as_source(){
    cairo_set_source_rgba(_cr, brush().color().redf(), brush().color().greenf(), brush().color().bluef(), brush().color().alphaf());
}

void cairo_paintengine::matrix_changed(const hsm::matrix3x3 & m){
    if(!device())
        return;
    cairo_matrix_t cm = {m(0, 0), m(1, 0), m(0, 1), m(1, 1), m(0, 2), m(1, 2)};
    cairo_set_matrix(_cr, &cm);
}

void cairo_paintengine::draw_points(const hsm::point *point, int n){
    if(!device())
        return;
    set_brush_as_source();
    for(int i = 0; i < n; ++i){
        cairo_rectangle(_cr, (double)point->x(), (double)point->y(), 1.0, 1.0);
    }
    cairo_fill(_cr);
}

void cairo_paintengine::draw_points(const pointf *point, int n){
    if(!device())
        return;
    set_brush_as_source();
    for(int i = 0; i < n; ++i){
        cairo_rectangle(_cr, (double)point->x(), (double)point->y(), 1.0, 1.0);
    }
    cairo_fill(_cr);
}

void cairo_paintengine::draw_rects(const rectf *rect, int n){
    if(!device())
        return;
    set_brush_as_source();
    for(int i = 0; i < n; ++i){
        cairo_rectangle(_cr, (double)rect->x(), (double)rect->y(), (double)rect->width(), (double)rect->height());
    }
    cairo_fill(_cr);
}

void cairo_paintengine::draw_rects(const hsm::rect *rect, int n){
    if(!device())
        return;
    set_brush_as_source();
    for(int i = 0; i < n; ++i){
        cairo_rectangle(_cr, (double)rect->x(), (double)rect->y(), (double)rect->width(), (double)rect->height());
    }
    cairo_fill(_cr);
}

void cairo_paintengine::draw_bitmap(const rect &target, const bitmap &bitmap, const rect &source){
    if(!device())
        return;
    cairo_surface_t *src = cairo_image_surface_create_for_data(const_cast<unsigned char *>(bitmap.data()), cairo_format(bitmap.format()), bitmap.width(), bitmap.height(), bitmap.stride());

    cairo_set_source_surface(_cr, src, 0.0, 0.0);
    cairo_rectangle(_cr, (double)target.x(), (double)target.y(), (double)target.width(), (double)target.height());
    cairo_fill(_cr);
}

}
