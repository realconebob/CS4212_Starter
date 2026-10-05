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
class DiffuseShader: public Material<DiffuseShader<T, X>, T, X> {
    public:
    DiffuseShader(const VecX<T, X>& albedo = VecX<T, X>::Ones()) {
        this->reflection_ = albedo;
    }

    [[nodiscard]] bool scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered, const std::vector<LightAny<T, X>>& lights) const {
        return _scatter(rayin, record, atten, scattered, lights);
    }

    // There's a way to make this private but idc right now
    [[nodiscard]] bool _scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered, const std::vector<LightAny<T, X>>& lights) const {
        auto scatter_direction = record.normal() + randunitv<T, X>();
        if(scatter_direction.near_zero()) scatter_direction = record.normal();

        scattered = RayX<T, X>{record.point(), scatter_direction};
        atten = this->reflection_;
        return true;
    }
};

}
#endif