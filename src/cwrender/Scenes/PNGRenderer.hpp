/**
 * @file PNGRenderer.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Renders a framebuffer to png
 * @version 0.1
 * @date 2026-09-26
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS__PNG_RENDERER__3008151373543__
#define CS4212_GRAPHICS__PNG_RENDERER__3008151373543__

#include "cwrender/Helpers.hpp"
#include "cwrender/Scenes/Framebuffer.hpp"

#include "png++/image.hpp"
#include "png++/rgb_pixel.hpp"

#include <string>

namespace cwrender {

template<Floating T>
class PNGRenderer {
    private:
        const Framebuffer<T, 3>& framebuffer_;
        const std::string path_;

    public:
        PNGRenderer(const Framebuffer<T, 3>& fb, std::string path):
            framebuffer_{fb}, path_{std::move(path)} {}

        /**
         * @brief Render the framebuffer to the respectively named png
         */
        void render() const {
            std::size_t
                w = framebuffer_.width(),
                h = framebuffer_.height();

            VecX<T, 3> fbpix;
            png::image<png::rgb_pixel> imgData(w, h); // weird syntax

            for (std::size_t y = 0; y < imgData.get_height(); ++y) {
            for (std::size_t x = 0; x < imgData.get_width(); ++x) {
                fbpix = framebuffer_(x, y) * 255.0;
                T
                    r = fbpix[0], //linear_to_gamma<T>(fbpix[0]),
                    g = fbpix[1], //linear_to_gamma<T>(fbpix[1]),
                    b = fbpix[2]; //linear_to_gamma<T>(fbpix[2]);

                imgData[y][x] = png::rgb_pixel(r, g, b);
	        }}
            imgData.write(path_);

            return;
        }
};

}
#endif