#ifndef CS4212_GRAPHICS__PNG_RENDERER__3008151373543__
#define CS4212_GRAPHICS__PNG_RENDERER__3008151373543__

#include "helpers.h"
#include "framebuffer.h"

#include "png++/image.hpp"
#include "png++/rgb_pixel.hpp"

#include <string>

template<Floating T>
class PNGRenderer {
    private:
        const Framebuffer<T, 3>& framebuffer_;    
        const std::string path_;
        
    public:
        PNGRenderer<T>(const Framebuffer<T, 3>& fb, std::string path):
            framebuffer_{fb}, path_{std::move(path)} {} 

        bool render() const {
            std::size_t
                w = framebuffer_.width(),
                h = framebuffer_.height();

            VecX<T, 3> fbpix;
            png::image<png::rgb_pixel> imgData(w, h); // weird syntax
            for(std::size_t y = 0; y < h; y++) {
                for(std::size_t x = 0; x < w; x++) {
                    fbpix = framebuffer_(x, y) * 255.0;
                    imgData[y][x] = png::rgb_pixel(fbpix[0], fbpix[1], fbpix[2]);
                }
            }
            imgData.write(path_);

            return true;
        }
};

// ITS BROKEN ((((((((((((((((((((((((((((((((((((((((((((((((((((((((((

#endif