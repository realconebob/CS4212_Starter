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

#include <cstddef>
#include <initializer_list>
#include <vector>

namespace cwrender {

template<Floating T, std::size_t X>
class Scene: public Hittable<Scene<T, X>, T, X> {
    private:
    std::vector<LightAny<T, 3>> lights_;

    // This will be replaced with a BST eventually
    std::vector<HittableAny<T, X>> hittables_;

    public:
    Scene(): hittables_{}, lights_{} {}

    Scene(std::initializer_list<HittableAny<T, X>> hittables, std::initializer_list<LightAny<T, X>> lights): hittables_(hittables), lights_(lights) {}

    void hittable_front(const HittableAny<T, X>& obj) {hittables_.insert(hittables_.begin(), obj);}
    void hittable_back(const HittableAny<T, X>& obj) {hittables_.push_back(obj);}

    void light_front(const LightAny<T, X>& obj) {lights_.insert(lights_.begin(), obj);}
    void light_back(const LightAny<T, X>& obj) {lights_.push_back(obj);}

    const std::vector<LightAny<T, X>>& getlights() const {return lights_;}

    bool intersect_impl(const RayX<T, X>& ray, HitRecord<T, 3>& record, const Interval<T>& range) const {
        bool hit = false;
        T closest = range.max;
        for (const auto& obj: hittables_) {
            if (obj.intersect(ray, record, Interval<T>(range.min, closest))) {
                hit = true;
                closest = record.t();
            }
        }
        return hit;
    }

    BoundingBox<T, X> bounds_impl() const {
        throw NotImplemented();
    }
};

}
#endif