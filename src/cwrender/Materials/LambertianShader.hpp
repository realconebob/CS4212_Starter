/**
 * @file LambertianShader.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Lambertian shader / material implementation
 * @version 0.1
 * @date 2026-09-30
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_LAMBERTIAN__243655423749__
#define CS4212_GRAPHICS_LAMBERTIAN__243655423749__

#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/Materials/Material.hpp"
#include "cwrender/BaseTypes/RayX.hpp"
#include "cwrender/BaseTypes/VecX.hpp"
#include "cwrender/Scenes/Light.hpp"

#include <cstddef>

namespace cwrender {

template<Floating T, std::size_t X>
class LambertianShader: public Material<LambertianShader<T, X>, T, X> {
    public:
    LambertianShader(const VecX<T, X>& albedo = VecX<T, X>::Ones()) {
        this->reflection_ = albedo;
        this->override_atten_ = true;
    }

    [[nodiscard]] bool scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered,const std::vector<LightAny<T, X>>& lights) const {
        return _scatter(rayin, record, atten, scattered, lights);
    }

    // There's a way to make this private but idc right now
    [[nodiscard]] bool _scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered, const std::vector<LightAny<T, X>>& lights) const {
        auto color = VecX<T, X>::Zeros();
        for(const LightAny<T, X>& light: lights) {
            auto tmpnormal = (record.front_face()) ? record.normal() : -record.normal();
            auto tolight = unit(light.pos() - record.point());
            T dist2 = dot(tolight, tolight);
            auto brightness = max<T>(0, dot(tolight, unit(tmpnormal)));
            color += this->reflection_ * light.color() * (brightness / dist2);
        }
        atten = color;
        return true;
    }
};

}
#endif