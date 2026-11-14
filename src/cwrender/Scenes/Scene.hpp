/**
 * @file Scene.hpp
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief A generic holder of Hittable objects for rendering
 * @version 0.1
 * @date 2026-09-26
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_SCENE__7971888621546__
#define CS4212_GRAPHICS_SCENE__7971888621546__

#include "cwrender/Helpers.hpp"
#include "cwrender/Hittables/Hittables.hpp"
#include "cwrender/BaseTypes/Interval.hpp"
#include "cwrender/Scenes/Light.hpp"
#include "cwrender/Scenes/BVT.hpp"
#include "cwrender/Renderable.hpp"

#include <cstddef>
#include <memory>
#include <vector>

namespace cwrender {

template<Floating T, std::size_t X>
class Scene: public Hittable<T, X> {
    private:
    BoundingVolumeHierarchy<T, X> hittables_{};
    std::vector<std::unique_ptr<Light<T, 3>>> lights_{};

    template<Floating t, std::size_t x> friend class Hittable;

    public:
    Scene(): hittables_{}, lights_{} {}

    template <typename... Args>
    requires (sizeof...(Args) > 0 && !(std::same_as<std::remove_cvref_t<Args>, Scene> || ...))
    explicit Scene(Args&&... args) {
        (add(std::forward<Args>(args)), ...);
        hittables_.build_from_internal_list();
    }

    void add(Renderable<T, X> r) {hittables_.add_back(std::move(r)); }

    template <std::derived_from<Light<T, X>> L>
    void add(L l) { lights_.push_back(std::make_unique<L>(std::move(l))); }

    const std::vector<std::unique_ptr<Light<T, X>>>& getlights() const {return lights_;}



    bool intersect(const RayX<T, X>& ray, HitRecord<T, 3>& record, const Interval<T>& range) const override {
        return hittables_.intersect(ray, record, range);
    }

    BoundingBox<T, X> bounds() const override {
        throw NotImplemented();
    }
};

}
#endif