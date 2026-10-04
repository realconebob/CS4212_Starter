/**
 * @file PointLight.hpp
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Point light implementation
 * @version 0.1
 * @date 2026-10-04
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_POINTLIGHT__74361305524204__
#define CS4212_GRAPHICS_POINTLIGHT__74361305524204__

#include "cwrender/Helpers.hpp"
#include "cwrender/Scenes/Light.hpp"
#include <cstddef>

using namespace cwrender;
namespace cwrender {

template<Floating T, std::size_t X>
class PointLight: public Light<PointLight<T, X>, T, X> {
    public:
    PointLight(VecX<T, X> pos, VecX<T, X> color, VecX<T, X> intensity) {
        this->pos_ = pos;
        this->color_ = color;
        this->intensity_ = intensity;
    }
};

}
#endif