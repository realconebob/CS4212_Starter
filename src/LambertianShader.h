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
    public:
    [[nodiscard]] static VecX<T, X> raycolor(const RayX<T, X>& ray, const HitRecord<T, X>& record, int depth) {
        if(depth <= 0) return VecX<T, X>{};

        auto dir = record.normal() + randomunitv<T, X>();
        return T(0.5) * raycolor(RayX<T, X>{record.point(), dir}, record, depth - 1);
    }
};

#endif