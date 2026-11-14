/**
 * @file Material.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Material implementation
 * @version 0.1
 * @date 2026-09-29
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_MATERIAL__3962847032450__
#define CS4212_GRAPHICS_MATERIAL__3962847032450__

#include "cwrender/Helpers.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/BaseTypes/VecX.hpp"
#include "cwrender/Scenes/Scene.hpp"

#include <cstddef>

namespace cwrender {

template<Floating T, std::size_t X>
class Material {
    protected:
    Material() = default;

    Material& operator=(const Material&) = delete;
    Material(const Material&) = delete;
    Material(Material&&) = delete;

    /**
    @brief The amount of color that should be reflected, channelwise. `VecX<T, 3>{1, 1, 1}` would
    reflect all light, `VecX<T, 3>{0, 0, 0}` would absorb all light, `VecX<T, 3>{1, 0, 0}` would absorb
    all non-red light

    @note A green reflecting sphere being hit with no green light will look black. A white reflecting
    sphere being hit with only green light will look green. reflection is not color, it modulates what
    channels of light get reflected
    */
    VecX<T, X> reflection_{VecX<T, X>::Ones()};

    /**
     * @brief Whether or not to override a black color with the attenuation of the shader on raycolor
     *
     */
    bool override_atten_{false};

    public:
    virtual ~Material() = default;

    bool scatter(const Scene<T, X>& scene, const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten_out, RayX<T, X>& scattered_out) const = 0;

    const VecX<T, X>& reflection() const {return reflection_;}
    bool override_atten() const {return override_atten_;}
};

}
#endif