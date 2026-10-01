/**
 * @file NormalShader.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief NormalMap Shader Implementation
 * @version 0.1
 * @date 2026-09-30
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_NORMALMAP__24730147238262__
#define CS4212_GRAPHICS_NORMALMAP__24730147238262__

#include "cwrender/Helpers.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/BaseTypes/RayX.hpp"
#include "cwrender/BaseTypes/VecX.hpp"

#include <cstddef>

namespace cwrender {

template<Floating T, std::size_t X>
class NormalMapShader {
    public:
    [[nodiscard]] static VecX<T, X> raycolor(const RayX<T, X>& ray, const HitRecord<T, X>& record) {
        return 0.5 * (record.normal() + Vec3D::Ones());
    }
};

}
#endif