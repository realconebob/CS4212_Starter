/**
 * @file World.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief A generic holder of Hittable objects for rendering
 * @version 0.1
 * @date 2026-09-26
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef CS4212_GRAPHICS_WORLD__7971888621546__
#define CS4212_GRAPHICS_WORLD__7971888621546__

#include "Hittable.h"
#include <concepts>
#include <vector>

template<Floating T>
class World3D: public Hittable<World3D<T>, T, 3> {
    private:
    // This will be replaced with a BST eventually
    std::vector<HittableAny<T, 3>> storage_;

    public:
    World3D(): storage_{} {}

    template<typename... Args>
    requires (std::same_as<Args, HittableAny<T, 3>> && ...)
    World3D(Args... args): storage_{args...} {}

    void push_front(const HittableAny<T, 3>& obj) {storage_.insert(0, obj);}
    void push_back(const HittableAny<T, 3>& obj) {storage_.push_back(obj);}

    bool _intersect(const RayX<T, 3>& ray) const {
        for(const HittableAny<T, 3>& obj: storage_) {
            // Not ideal, but the result of intersect will change from "does an intersection happen" to "what is intersecting" sooner than later, so not a big deal rn
            if(obj.intersect(ray)) return true;
        }
        return false;
    }
};

#endif