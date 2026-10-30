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

#include "cwrender/Helpers.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/Hittables/Triangle.hpp"

namespace cwrender {

template<Floating T>
class Plane3: public Hittable<Plane3<T>, T, 3> {
    private:
    Triangle3<T> tris_[2];
    std::shared_ptr<MaterialAny<T, 3>> mat_;

    public:
    /// TODO: This really should be bottom-left and top-right, given that this exists in 3d space not screen space
    Plane3(const VecX<T, 3>& topleft, const VecX<T, 3>& bottomright, std::shared_ptr<MaterialAny<T, 3>> mat): tris_{Triangle3<T>{}, Triangle3<T>{}}, mat_(mat) {
        // Note: think about how to ensure z axis isn't leading to wackiness

        VecX<T, 3>
            topright = {bottomright[0], topleft[1], topleft[2]}, // TODO: CHECK Z AXIS
            bottomleft = {topleft[0], bottomright[1], bottomright[2]}; // TODO: CHECK Z AXIS

        Triangle3<T>
            t1 = Triangle3<T>(topleft, topright, bottomleft, mat_), // A -> B -> C: top-left, top-right, bottom-left
            t2 = Triangle3<T>(bottomleft, topright, bottomright, mat_); // A -> B -> C: bottom-left, top-right, bottom-right

        tris_[0] = t1;
        tris_[1] = t2;
    }

    static Plane3<T> empty() {
        return Plane3<T>{{}, {}};
    }

    bool intersect_impl(const RayX<T, 3>& ray, HitRecord<T, 3>& record, const Interval<T>& range) const {
        for(int i = 0; i < 2; i++) if(tris_[i].intersect(ray, record, range)) return true;
        return false;
    }

    BoundingBox<T, 3> bounds_impl() const {
        throw NotImplemented("TODO: Compare the bounds of the bounding boxes of either plane to get the new bounds");
    }
};

}
#endif