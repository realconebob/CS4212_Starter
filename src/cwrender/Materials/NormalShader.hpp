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
#include "cwrender/Materials/Material.hpp"

#include <cstddef>

namespace cwrender {

template<Floating T, std::size_t X>
class NormalMapShader: public Material<NormalMapShader<T, X>, T, X> {
    public:
    NormalMapShader(const VecX<T, X>& albedo = VecX<T, X>::Ones()) {}

    [[nodiscard]] bool scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered) const {
        return _scatter(rayin, record, atten, scattered);
    }

    // There's a way to make this private but idc right now
    [[nodiscard]] bool _scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered) const {
        atten = 0.5 * (record.normal() + VecX<T, X>::Ones());
        return false;
    }
};

}
#endif