/**
 * @file HittableFact.hpp
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Hittable factory implementation
 * @version 0.1
 * @date 2026-10-27
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_HITTABLEFACT__93993911667__
#define CS4212_GRAPHICS_HITTABLEFACT__93993911667__

#include "cwrender/Helpers.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/Hittables/Plane.hpp"
#include "cwrender/Hittables/Sphere.hpp"
#include "cwrender/Hittables/Triangle.hpp"

#include <cstddef>
#include <stdexcept>
#include <unordered_map>

namespace cwrender {

template<Floating T, std::size_t X>
class HittableFactory {
    public:
    enum TYPE {
        SPHERE,
        TRIANGLE,
        PLANE,
        CUBE
    };

    HittableAny<T, X> build(TYPE type, const std::unordered_map<const char*, const char*>& data) {
        std::shared_ptr<MaterialAny<T, 3>> mat;
        switch (type) {
            default: throw std::invalid_argument("Given invalid hittable type");

            case SPHERE:
            // Get/build from map
            VecX<T, 3> origin;
            T radius;
            std::shared_ptr<MaterialAny<T, 3>> material;
            return Sphere3D<T>{origin, radius, material}.any();


            case TRIANGLE:
            // Get/build from map
            VecX<T, 3> a, b, c;
            mat;
            return Triangle3<T>{a, b, c, mat}.any();


            case PLANE:
            VecX<T, 3> topleft, bottomright;
            mat;
            return Plane3<T>{topleft, bottomright, mat}.any();


            case CUBE:
            throw NotImplemented("The base cube class is still not implemented lol");
        }

        throw std::logic_error("<HittableFact::build>: Broke out of switch statement");
    }
};

}
#endif