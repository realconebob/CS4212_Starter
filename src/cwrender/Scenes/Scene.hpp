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

#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/BaseTypes/Interval.hpp"
#include "cwrender/Scenes/Light.hpp"

#include <initializer_list>
#include <vector>

namespace cwrender {

template<Floating T>
class Scene3: public Hittable<Scene3<T>, T, 3>, public Light<Scene3<T>, T, 3> {
    private:
    std::vector<LightAny<T, 3>> lights_;

    // This will be replaced with a BST eventually
    std::vector<HittableAny<T, 3>> hittables_;

    public:
    Scene3(): hittables_{}, lights_{} {}

    Scene3(std::initializer_list<HittableAny<T, 3>> hittables, std::initializer_list<LightAny<T, 3>> lights): hittables_(hittables), lights_(lights) {}

    void hittable_front(const HittableAny<T, 3>& obj) {hittables_.insert(hittables_.begin(), obj);}
    void hittable_back(const HittableAny<T, 3>& obj) {hittables_.push_back(obj);}

    void light_front(const LightAny<T, 3>& obj) {lights_.insert(lights_.begin(), obj);}
    void light_back(const LightAny<T, 3>& obj) {lights_.push_back(obj);}


    bool _intersect(const RayX<T, 3>& ray, HitRecord<T, 3>& record, const Interval<T>& range) const {
        for(const HittableAny<T, 3>& obj: hittables_) {
            if(obj.intersect(ray, record, range)) return true;
        }
        return false;
    }
};

}
#endif