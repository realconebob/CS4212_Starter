/**
 * @file MaterialFact.hpp
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Material factory implementation
 * @version 0.1
 * @date 2026-10-27
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_MATERIALFACT__30759773618898__
#define CS4212_GRAPHICS_MATERIALFACT__30759773618898__

#include "cwrender/Materials/BlinnPhongShader.hpp"
#include "cwrender/Materials/DiffuseShader.hpp"
#include "cwrender/Materials/LambertianShader.hpp"
#include "cwrender/Materials/Material.hpp"
#include "cwrender/Materials/MetalShader.hpp"
#include "cwrender/Materials/NormalShader.hpp"

#include <cstddef>
#include <stdexcept>
#include <unordered_map>

namespace cwrender {

template<Floating T, std::size_t X>
class MaterialFact {
    public:
    enum TYPE {
        PHONG,
        DIFFUSE,
        LAMBERTIAN,
        METAL,
        NORMAL,
    };

    MaterialAny<T, X> build(TYPE type, const std::unordered_map<const char*, const char*>& data) {
        const VecX<T, X>& albedo = VecX<T, X>::Ones();
        T shiny;
        T fuzz = T(0.0);

        switch (type) {
            default: throw std::invalid_argument("<MaterialFact::build>: Given invalid material type");

            case PHONG:
            // Populate from map
            VecX<T, X> spec_reflection;
            shiny;
            albedo;
            return BlinnPhongShader<T, X>{spec_reflection, shiny, albedo}.any();


            case DIFFUSE:
            // Populate from map
            albedo;
            return DiffuseShader<T, X>{albedo}.any();


            case LAMBERTIAN:
            // Populate from map
            albedo;
            return LambertianShader<T, X>{albedo}.any();


            case METAL:
            // Populate from map
            albedo;
            fuzz;
            return MetalShader<T, X>{albedo, fuzz}.any();


            case NORMAL:
            return NormalMapShader<T, X>{}.any();
        }

        throw std::logic_error("<MaterialFact::build>: Broke out of switch statement");
    }
};

}
#endif