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

#include <cstddef>
#include <initializer_list>
#include <vector>

namespace cwrender {

template<Floating T, std::size_t X>
class Scene: public Hittable<T, X> {
    private:
    BoundingVolumeHierarchy<T, X> hittables_;
    std::vector<Light<T, 3>> lights_;

    template<Floating t, std::size_t x> friend class Hittable;

    public:
    Scene(): hittables_{}, lights_{} {}

    Scene(
        std::initializer_list<Hittable<T, X>> hittables,
        std::initializer_list<Light<T, X>> lights
    ): hittables_(hittables), lights_(lights) {}

    void hittable_front(const Hittable<T, X>& obj) {hittables_.add_front(obj);}
    void hittable_back(const Hittable<T, X>& obj) {hittables_.add_back(obj);}

    void light_front(const Light<T, X>& obj) {lights_.insert(lights_.begin(), obj);}
    void light_back(const Light<T, X>& obj) {lights_.push_back(obj);}
    const std::vector<Light<T, X>>& getlights() const {return lights_;}



    bool intersect(const RayX<T, X>& ray, HitRecord<T, 3>& record, const Interval<T>& range) const override {
        return hittables_.intersect(ray, record, range);
    }

    BoundingBox<T, X> bounds_impl() const override {
        throw NotImplemented();
    }
};

}
#endif