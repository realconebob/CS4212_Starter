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
#include <memory>

namespace cwrender {

template<Floating T>
class Plane3: public Hittable<T, 3> {
    private:
    template<Floating t, std::size_t x> friend class Hittable;

    Triangle3<T> tris_[2]{0};

    public:
    Plane3(const VecX<T, 3>& bottomleft, const VecX<T, 3>& topright): tris_{Triangle3<T>{}, Triangle3<T>{}} {
        VecX<T, 3>
            topleft{bottomleft[0], topright[1], bottomleft[2]},     /// TODO: CHECK Z AXIS
            bottomright{topright[0], bottomleft[1], topright[2]};   /// TODO: CHECK Z AXIS

        Triangle3<T>
            t1{bottomleft, topleft, topright},     // TODO: CHECK NORMALS
            t2{topright, bottomright, bottomleft}; // TODO: CHECK NORMALS

        tris_[0] = t1;
        tris_[1] = t2;
    }

    static Plane3<T> empty() {
        return Plane3<T>{{}, {}};
    }

    bool intersect(const RayX<T, 3>& ray, HitRecord<T, 3>& record, const Interval<T>& range) const override {
        for(int i = 0; i < 2; i++) if(tris_[i].intersect(ray, record, range)) return true;
        return false;
    }

    BoundingBox<T, 3> bounds() const override {
        return BoundingBox<T, 3>{tris_[0].bounds(), tris_[1].bounds()};
    }
};

}
#endif