#include "image.h"
#include <png.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <hsm/bytearray.h>
#include "image_paintengine.h"
#include "cairo_paintengine.h"

#include "private/bitmap_png.h"
#include "private/bitmap_bmp.h"
#include "private/bitmap_jpeg.h"

namespace hsm{

image::image(){
    //
}

image::image(size_t width, size_t height, bitmap::pixel_format format) : bitmap(width, height, format){
    //
}

image::image(size_t width, size_t height, bitmap::pixel_format format, size_t stride) : bitmap(width, height, format, stride == 0 ? image::prefered_stride(format, width) : stride){
    //
}

image::image(uint8_t *data, size_t width, size_t height, bitmap::pixel_format format, bool copy) : bitmap(data, width, height, format, copy){
    //
}

image::image(uint8_t *data, size_t width, size_t height, bitmap::pixel_format format, size_t stride, bool copy) : bitmap(data, width, height, format, stride == 0 ? image::prefered_stride(format, width) : stride, copy){
    //
}

image::image(const hsm::bitmap &bmp) : bitmap(bmp){
    //
}

image::image(const std::string & filename) : bitmap(){
    // Read file
    std::ifstream file(filename);
    if(!file.good()){
        std::cerr << "cannot open file `" << filename << "'" << std::endl;
        return;
    }
    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);
    bytearray data;
    data.reserve(length);
    data.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());
    file.close();

    // Read image
    bool ok = false;
    if(!ok) // PNG
        _data = fromPNG(data, &ok, &_format, &_width, &_height);
    /*if(!ok) // BMP
        _data = fromBMP(data, &ok, &_format, &_internal_format, &_width, &_height);*/
    if(!ok)
        _data = fromJPEG(data, &ok, &_format, &_width, &_height);
    if(!ok){
        std::cerr << "unknown image format for file `" << filename << "'" << std::endl;
        if(_data)
            delete[] _data;
    }
}

size_t image::width() const{
    return bitmap::width();
}

size_t image::height() const{
    return bitmap::height();
}

size_t image::prefered_stride(bitmap::pixel_format f, size_t width){
    return cairo_paintengine::prefered_stride(f, width);
}

paintengine *image::paint_engine() const{
    if(cairo_paintengine::is_valid_format(format()))
        return new cairo_paintengine(_data, width(), height(), format(), stride());
    return new image_paintengine;
}

}
