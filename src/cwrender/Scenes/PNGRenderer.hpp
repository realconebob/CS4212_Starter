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
        void render(bool gamma_correction = false) const {
            std::size_t
                w = framebuffer_.width(),
                h = framebuffer_.height();

            VecX<T, 3> corrected, fbpix;
            png::image<png::rgb_pixel> imgData(w, h); // weird syntax

            for (std::size_t y = 0; y < imgData.get_height(); ++y) {
            for (std::size_t x = 0; x < imgData.get_width(); ++x) {
                corrected = (gamma_correction)
                    ? linear_to_srgb(framebuffer_(x, y))
                    : framebuffer_(x, y);

                fbpix = corrected * T(255.0);// + (T(0.5) * VecX<T, 3>::Ones());

                imgData[y][x] = png::rgb_pixel(fbpix[0], fbpix[1], fbpix[2]);
	        }}
            imgData.write(path_);

            return;
        }
};

}
#endif