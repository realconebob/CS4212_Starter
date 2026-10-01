/**
 * @file Triangle.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Triangle implementation
 * @version 0.1
 * @date 2026-09-28
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_TRIANGLE__14195264447411__
#define CS4212_GRAPHICS_TRIANGLE__14195264447411__

#include "cwrender/Helpers.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/BaseTypes/RayX.hpp"

#include <limits>

namespace cwrender {

template<Floating T>
class Triangle3: public Hittable<Triangle3<T>, T, 3> {
    private:
    VecX<T, 3> a_, b_, c_;
    VecX<T, 3> e1_, e2_, normal_;
    std::shared_ptr<MaterialAny<T, 3>> mat_;

    public:
    explicit Triangle3() {}
    Triangle3(VecX<T, 3> a, VecX<T, 3> b, VecX<T, 3> c, std::shared_ptr<MaterialAny<T, 3>> mat):
        a_{a}, b_{b}, c_{c},
        e1_{b - a}, e2_{c - a},
        normal_{unit(cross(e1_, e2_))},
        mat_(mat)
        {}

    bool _intersect(const RayX<T, 3>& ray, HitRecord<T, 3>& record, const Interval<T>& range) const {
        // Got this from claude because the slides were not clear on how to do this
        constexpr T eps = std::numeric_limits<T>::epsilon() * 10;

        VecX<T, 3> p = cross(ray.dir(), e2_);
        T det = dot(e1_, p);
        if (std::abs(det) < eps) return false; // parallel to the triangle's plane

        T inv = T(1) / det;
        VecX<T, 3> s = ray.origin() - a_;
        T u = dot(s, p) * inv;
        if (u < 0 || u > 1) return false;

        VecX<T, 3> q = cross(s, e1_);
        T v = dot(ray.dir(), q) * inv;
        if (v < 0 || u + v > 1) return false;

        T t = dot(e2_, q) * inv;
        if(!range.surrounds(t)) return false;

        VecX<T, 3> point = ray.origin() + t * ray.dir();
        this->update(record, point, t, ray, normal_, mat_);
        return true;
    }
};

}
#endif