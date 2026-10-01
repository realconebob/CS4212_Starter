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

#ifndef CS4212_GRAPHICS_LAMBERTIAN__25048403216100__
#define CS4212_GRAPHICS_LAMBERTIAN__25048403216100__

#include "Hittable.h"
#include "Material.h"
#include "RayX.h"
#include "VecX.h"
#include <cstddef>

template<Floating T, std::size_t X>
class Lambertian {
    private:
    VecX<T, X> reflection_;

    [[nodiscard]] bool _scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered) const {
        auto scatter_direction = record.normal + randomunitv<T, X>();
        if(scatter_direction.near_zero())
            scatter_direction = record.normal();

        scattered = RayX<T, X>{record.point(), scatter_direction};
        atten = reflection_;
        return true;
    }

    public:
    Lambertian(const VecX<T, X>& albedo = VecX<T, X>::Ones()): reflection_(albedo) {}

        [[nodiscard]] bool scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered) const {
            return _scatter(rayin, record, atten, scattered);
        }
};

#endif