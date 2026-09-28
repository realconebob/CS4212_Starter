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

#include "Hittable.h"
#include "RayX.h"
#include "VecX.h"
#include <cmath>

template<Floating T>
class Sphere3D: public Hittable<Sphere3D<T>, T, 3> {
    private:
    VecX<T, 3> origin_;
    T radius_;

    public:
    Sphere3D(const VecX<T, 3>& origin, T radius):
        origin_{origin}, radius_{std::abs(radius)}
        {}

    bool _intersect(const RayX<T, 3>& ray, HitRecord<T, 3>& record) const {
        VecX<T, 3> oc = origin_ - ray.origin();

        T
            a = ray.dir().length_squared(),
            h = dot(ray.dir(), oc),
            c = oc.length_squared() - radius_*radius_,
            discrim = h*h - a*c;

        if(discrim < 0) return false;

        auto sqrtd = std::sqrt(discrim);

        // TODO: Replace these with the interval class when ready. Not doing it now because I JUST changed intersect's signature and I don't want to again
        const T ray_tmin = -100;
        const T ray_tmax = -ray_tmin;

        auto root = (h - sqrtd) / a;
        if (root <= ray_tmin || ray_tmax <= root) {
            root = (h + sqrtd) / a;
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }
        auto p = ray.at(root);
        VecX<T, 3> out_normal = (p - origin_) / radius_;
        this->update(record, p, root, ray, out_normal);

        return true;
    }
};

#endif