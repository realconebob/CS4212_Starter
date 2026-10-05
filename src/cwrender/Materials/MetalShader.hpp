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

#ifndef CS4212_GRAPHICS_METAL__200783036924283__
#define CS4212_GRAPHICS_METAL__200783036924283__

#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/Materials/Material.hpp"
#include "cwrender/BaseTypes/RayX.hpp"
#include "cwrender/BaseTypes/VecX.hpp"

#include <cstddef>

namespace cwrender {

template<Floating T, std::size_t X>
class MetalShader: public Material<MetalShader<T, X>, T, X> {
    private:
    T fuzz_;

    public:
    MetalShader(const VecX<T, X>& albedo = VecX<T, X>::Ones(), T fuzz = 0.0): fuzz_(fuzz) {
        this->reflection_ = albedo;
    }

    [[nodiscard]] bool scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered, const std::vector<LightAny<T, X>>& lights) const {
        return _scatter(rayin, record, atten, scattered, lights);
    }

    // There's a way to make this private but idc right now
    [[nodiscard]] bool _scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered, const std::vector<LightAny<T, X>>& lights) const {
        VecX<T, X> reflected = reflect(rayin.dir(), record.normal());
        reflected = unit(reflected) + (fuzz_ * randunitv<T, X>());

        scattered = RayX<T, X>(record.point(), reflected);
        atten = this->reflection_;
        return (dot(scattered.dir(), record.normal()) > 0);
    }
};

}
#endif