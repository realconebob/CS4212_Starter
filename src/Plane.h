/**
 * @file Plane.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Plane implementation
 * @version 0.1
 * @date 2026-09-28
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_PLANE__21057231487604__
#define CS4212_GRAPHICS_PLANE__21057231487604__

#include "Helpers.h"
#include "Hittable.h"
#include "RayX.h"
#include "Triangle.h"

#include <limits>

template<Floating T>
class Plane3: public Hittable<Plane3<T>, T, 3> {
    private:
    Triangle3<T> tris_[2];

    public:
    Plane3(const VecX<T, 3>& topleft, const VecX<T, 3>& bottomright): tris_{Triangle3<T>{}, Triangle3<T>{}} {
        // Note: think about how to ensure z axis isn't leading to wackiness

        VecX<T, 3>
            topright = {bottomright[0], topleft[1], topleft[2]}, // TODO: CHECK Z AXIS
            bottomleft = {topleft[0], bottomright[1], bottomright[2]}; // TODO: CHECK Z AXIS

        Triangle3<T>
            t1 = Triangle3<T>(topleft, topright, bottomleft), // A -> B -> C: top-left, top-right, bottom-left
            t2 = Triangle3<T>(bottomleft, topright, bottomright); // A -> B -> C: bottom-left, top-right, bottom-right

        tris_[0] = t1;
        tris_[1] = t2;
    }

    bool _intersect(const RayX<T, 3>& ray, HitRecord<T, 3>& record, const Interval<T>& range) const {
        for(int i = 0; i < 2; i++) if(tris_[i].intersect(ray, record, range)) return true;
        return false;
    }
};

#endif