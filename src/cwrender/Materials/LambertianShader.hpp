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
#include <memory>

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
        for(const std::unique_ptr<Light<T, X>>& lightptr: scene.getlights()) {
            auto light = lightptr.get();
            VecX<T, X> tolight = light->pos() - record.point();
            auto dist = tolight.length();
            tolight = unit(tolight);

            auto brightness = max<T>(0, dot(tolight, unit(record.normal())));
            if(brightness <= 0) continue;

            HitRecord<T, X> newrec{};
            if(scene.intersect({record.point() + T(1e-4) * record.normal(), tolight}, newrec, Interval<T>{T(1e-4), dist})) continue;

            color += this->reflection_ * light->color() * brightness;
        }
        atten_out = color;
        return true;
    }
};

}
#endif