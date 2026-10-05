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
    NormalMapShader() {
        this->reflection_ = VecX<T, X>::Ones(); // Not entirely necessary but whatever
        this->override_atten_ = true;
    }

    [[nodiscard]] bool scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered, const std::vector<LightAny<T, X>>& lights) const {
        return _scatter(rayin, record, atten, scattered, lights);
    }

    // There's a way to make this private but idc right now
    [[nodiscard]] bool _scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered, const std::vector<LightAny<T, X>>& lights) const {
        atten = 0.5 * (record.normal() + VecX<T, X>::Ones());
        return false;
    }
};

}
#endif