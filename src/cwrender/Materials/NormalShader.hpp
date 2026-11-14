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
class NormalMapShader: public Material<T, X> {
    public:
    NormalMapShader() {
        this->reflection_ = VecX<T, X>::Ones(); // Not entirely necessary but whatever
        this->override_atten_ = true;
    }

    [[nodiscard]] bool scatter(const Scene<T, X>& scene, const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten_out, RayX<T, X>& scattered_out) const override {
        atten_out = 0.5 * (record.normal() + VecX<T, X>::Ones());
        return true;
    }
};

}
#endif