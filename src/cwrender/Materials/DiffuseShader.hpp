/**
 * @file DiffuseShader.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Diffuse shader / material implementation
 * @version 0.1
 * @date 2026-09-30
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_DIFFUSE__25048403216100__
#define CS4212_GRAPHICS_DIFFUSE__25048403216100__

#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/Materials/Material.hpp"
#include "cwrender/BaseTypes/RayX.hpp"
#include "cwrender/BaseTypes/VecX.hpp"

#include <cstddef>

namespace cwrender {

template<Floating T, std::size_t X>
class DiffuseShader: public Material<T, X> {
    public:
    DiffuseShader(const VecX<T, X>& albedo = VecX<T, X>::Ones()) {
        this->reflection_ = albedo;
    }

    [[nodiscard]] bool scatter(const Scene<T, X>& scene, const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten_out, RayX<T, X>& scattered_out) const override {
        auto scatter_direction = record.normal() + randunitv<T, X>();
        if(scatter_direction.near_zero()) scatter_direction = record.normal();

        scattered_out = RayX<T, X>{record.point(), scatter_direction};
        atten_out = this->reflection_;
        return true;
    }
};

}
#endif