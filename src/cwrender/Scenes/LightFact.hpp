/**
 * @file LightFact.hpp
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Light factory implementation
 * @version 0.1
 * @date 2026-10-27
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_LIGHTFACT__19282291372333__
#define CS4212_GRAPHICS_LIGHTFACT__19282291372333__

#include "cwrender/Scenes/Light.hpp"
#include "cwrender/Scenes/PointLight.hpp"
#include <cstddef>
#include <stdexcept>
#include <unordered_map>

namespace cwrender {

template<Floating T, std::size_t X>
class LightFactory {
    public:
    enum TYPE {
        UNDEF,
        POINT,
        TOOBIG,
    };

    static LightAny<T, X> build(TYPE type, const std::unordered_map<std::string, std::string>& data) {
        switch (type) {
            default: throw std::invalid_argument("<LightFactory::build>: Given invalid light type");

            case POINT:
            return [&data](){
                // Populate from map
                VecX<T, X> pos, color;

                return PointLight<T, X>{pos, color}.any();
            }();
        }

        throw std::logic_error("<LightFactory::build>: Broke out of switch statement");
    }
};

}
#endif