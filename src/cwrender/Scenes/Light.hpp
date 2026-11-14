/**
 * @file Light.hpp
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Light implementation
 * @version 0.1
 * @date 2026-10-04
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_LIGHT__19222122927224__
#define CS4212_GRAPHICS_LIGHT__19222122927224__

#include "cwrender/BaseTypes/VecX.hpp"
#include "cwrender/Helpers.hpp"
#include <cstddef>

namespace cwrender {

template<Floating T, std::size_t X>
class Light {
    protected:
    VecX<T, X> pos_{}, color_{};

    Light() = default;
    Light& operator=(const Light&) = delete;
    Light(const Light&) = delete;
    Light(Light&&) = delete;

    public:
    virtual ~Light() = default;
    VecX<T, X> pos() const {return pos_;}
    VecX<T, X> color() const {return color_;}
};

}
#endif