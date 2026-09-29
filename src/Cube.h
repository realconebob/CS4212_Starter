/**
 * @file Cube.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Cube implementation
 * @version 0.1
 * @date 2026-09-28
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_CUBE__28344179127720__
#define CS4212_GRAPHICS_CUBE__28344179127720__

#include "Helpers.h"
#include "Hittable.h"
#include "Plane.h"

#include <limits>

template<Floating T>
class Cube3: public Hittable<Cube3<T>, T, 3> {
    private:
    Plane3<T> planes_[6];

    public:
    Cube3(VecX<T, 3> topleft, VecX<T, 3> bottomright):
        planes_{ // This is gross
            Plane3<T>::empty(), Plane3<T>::empty(), Plane3<T>::empty(),
            Plane3<T>::empty(), Plane3<T>::empty(), Plane3<T>::empty()
        } {
        throw NotImplemented();
    }

    bool _intersect(const RayX<T, 3>& ray, HitRecord<T, 3>& record, const Interval<T>& range) const {
        for(int i = 0; i < 2; i++) if(planes_[i].intersect(ray, record, range)) return true;
        return false;
    }
};

#endif