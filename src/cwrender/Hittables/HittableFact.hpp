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

#include "cwrender/BaseTypes/VecX.hpp"
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
    private:
    HittableFactory() = delete; // This should be enough to stop people from making factories instead of just using the function

    public:
    enum TYPE {
        UNDEF,
        SPHERE,
        TRIANGLE,
        PLANE,
        CUBE,
        TOOBIG,
    };

    static Hittable<T, X> build(TYPE type, const std::unordered_map<std::string, std::string>& data) {
        switch (type) {
            default: throw std::invalid_argument("Given invalid hittable type");

            case SPHERE:
            return [&data](){
                // Populate from map
                VecX<T, 3> origin;
                T radius;

                return Sphere3<T>{origin, radius};
            }();

            case TRIANGLE:
            return [&data](){
                // Populate from map
                VecX<T, 3> a, b, c;
                return Triangle3<T>{a, b, c};
            }();

            case PLANE:
            return [&data](){
                // Populate from map
                VecX<T, 3> topleft, bottomright;
                return Plane3<T>{topleft, bottomright};
            }();

            case CUBE:
            throw NotImplemented("The base cube class is still not implemented lol");
        }

        throw std::logic_error("<HittableFact::build>: Broke out of switch statement");
    }
};

}
#endif