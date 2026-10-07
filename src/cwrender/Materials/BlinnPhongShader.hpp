/**
 * @file BlinnPhongShader.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Blinn-Phong shader / material implementation
 * @version 0.1
 * @date 2026-09-30
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_BLINNPHONG__25246685721411__
#define CS4212_GRAPHICS_BLINNPHONG__25246685721411__

#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/Materials/Material.hpp"
#include "cwrender/BaseTypes/RayX.hpp"
#include "cwrender/BaseTypes/VecX.hpp"
#include "cwrender/Scenes/Light.hpp"

#include <cstddef>

namespace cwrender {

template<Floating T, std::size_t X>
class BlinnPhongShader: public Material<BlinnPhongShader<T, X>, T, X> {
    private:
    VecX<T, X> spec_reflection_;
    T shiny_;

    public:
    BlinnPhongShader(VecX<T, X> spec_reflection, T shiny_, const VecX<T, X>& albedo = VecX<T, X>::Ones()): spec_reflection_(spec_reflection), shiny_(shiny_) {
        this->reflection_ = albedo;
        this->override_atten_ = true;
    }

    [[nodiscard]] bool scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered,const std::vector<LightAny<T, X>>& lights) const {
        return _scatter(rayin, record, atten, scattered, lights);
    }

    // There's a way to make this private but idc right now
    [[nodiscard]] bool _scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered, const std::vector<LightAny<T, X>>& lights) const {
        auto viewing = unit(record.point() - rayin.origin());
        auto color = VecX<T, X>::Zeros();
        for(const LightAny<T, X>& light: lights) {
            auto tolight =light.pos() - record.point();
            T dist2 = tolight.length_squared();
            tolight = unit(tolight);
            auto radiance = light.color() / dist2;

            auto ndot = max<T>(0, dot(record.normal(), tolight));
            T spec = 0;
            if (ndot < 0) {
                spec = std::pow(max<T>(0, dot(record.normal(), unit(tolight + viewing))), shiny_);
            }

            color += radiance * (this->reflection_ * ndot + spec_reflection_ * spec);
        }
        atten = color;
        return true;
    }
};

}
#endif