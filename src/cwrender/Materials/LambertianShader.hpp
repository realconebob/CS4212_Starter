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

#include <cstddef>
#include <limits>

namespace cwrender {

template<Floating T, std::size_t X>
class LambertianShader: public Material<LambertianShader<T, X>, T, X> {
    public:
    LambertianShader(const VecX<T, X>& albedo = VecX<T, X>::Ones()) {
        this->reflection_ = albedo;
        // this->override_atten_ = true;
    }

    [[nodiscard]] bool scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered,const std::vector<LightAny<T, X>>& lights) const {
        return _scatter(rayin, record, atten, scattered, lights);
    }

    // There's a way to make this private but idc right now
    [[nodiscard]] bool _scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered, const std::vector<LightAny<T, X>>& lights) const {
        auto scatter_direction = record.normal() + randunitv<T, X>();
        if(scatter_direction.near_zero()) scatter_direction = record.normal();
        scattered = RayX<T, X>{record.point(), scatter_direction};

        // This needs to change based on the lights in the scene and where they're positioned. That means the lights in the scene, or at least the lights that affect this material, need to be passed to scatter
        atten = VecX<T, X>::Zeros();
        for(const LightAny<T, X>& light: lights) {
            // VecX<T, X> tolight = light.pos() - record.point();
            // T dist2 = dot(tolight, tolight);
            // T ndot = max<T>(T(0), dot<T, X>(record.normal(), unit(tolight)));

            // if(dist2 <= std::numeric_limits<T>::epsilon()) dist2 = std::numeric_limits<T>::epsilon();

            // atten += this->reflection_ * light.color() * light.intensity() * (ndot/dist2);

            atten += this->reflection_ * light.color() * light.intensity() * max<T>(0, dot(record.normal(), unit(light.pos() - record.point())));
        }
        atten = atten.clamp(0, 1);

        return true;
    }
};

}
#endif