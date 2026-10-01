/**
 * @file Sphere.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Sphere implementation
 * @version 0.1
 * @date 2026-09-26
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_SPHERE__27838395618845__
#define CS4212_GRAPHICS_SPHERE__27838395618845__

#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/BaseTypes/RayX.hpp"
#include "cwrender/BaseTypes/VecX.hpp"

#include <cmath>
#include <memory>

namespace cwrender {

template<Floating T>
class Sphere3D: public Hittable<Sphere3D<T>, T, 3> {
    private:
    VecX<T, 3> origin_;
    T radius_;
    std::shared_ptr<MaterialAny<T, 3>> mat_;

    public:
    Sphere3D(const VecX<T, 3>& origin, T radius, std::shared_ptr<MaterialAny<T, 3>> material):
        origin_{origin}, radius_{std::abs(radius)}, mat_(material)
        {}

    bool _intersect(const RayX<T, 3>& ray, HitRecord<T, 3>& record, const Interval<T>& range) const {
        VecX<T, 3> oc = origin_ - ray.origin();

        T
            a = ray.dir().length_squared(),
            h = dot(ray.dir(), oc),
            c = oc.length_squared() - radius_*radius_,
            discrim = h*h - a*c;

        if(discrim < 0) return false;

        auto sqrtd = std::sqrt(discrim);

        auto root = (h - sqrtd) / a;
        if (root <= range.min || range.max <= root) {
            root = (h + sqrtd) / a;
            if (root <= range.min || range.max <= root)
                return false;
        }
        auto p = ray.at(root);
        VecX<T, 3> out_normal = (p - origin_) / radius_;
        this->update(record, p, root, ray, out_normal, mat_);

        return true;
    }
};

}
#endif