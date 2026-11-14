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
class LambertianShader: public Material<T, X> {
    public:
    LambertianShader(const VecX<T, X>& albedo = VecX<T, X>::Ones()) {
        this->reflection_ = albedo;
        this->override_atten_ = true;
    }

    [[nodiscard]] bool scatter(const Scene<T, X>& scene, const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten_out, RayX<T, X>& scattered_out) const override {
        auto color = VecX<T, X>::Zeros();
        for(const std::unique_ptr<Light<T, X>>& light: scene.getlights()) {
            auto tmpnormal = (record.front_face()) ? record.normal() : -record.normal();
            auto tolight = light.get()->pos() - record.point();
            T dist2 = tolight.length_squared();
            tolight = unit(tolight);

            HitRecord<T, X> newrec{};
            if(scene.intersect({record.point() + T(1e-4) * tmpnormal, tolight}, newrec, Interval<T>{T(1e-4), tolight.length()})) continue;

            auto brightness = max<T>(0, dot(tolight, unit(tmpnormal)));
            color += this->reflection_ * light.get()->color() * (brightness / dist2);
        }
        atten_out = color;
        return true;
    }
};

}
#endif