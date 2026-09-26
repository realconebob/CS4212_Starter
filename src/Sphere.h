#ifndef CS4212_GRAPHICS_SPHERE__27838395618845__
#define CS4212_GRAPHICS_SPHERE__27838395618845__

#include "Hittable.h"
#include "RayX.h"

template<Floating T>
class Sphere3D: public Hittable<Sphere3D<T>, T, 3> {
    private:
    VecX<T, 3> origin_;
    T radius_;

    public:
    Sphere3D(const VecX<T, 3>& origin, T radius):
        origin_{origin}, radius_{std::abs(radius)}
        {}

    bool _intersect(const RayX<T, 3>& ray) const {
        VecX<T, 3> oc = origin_ - ray.origin();
        T
            a = dot(ray.dir(), ray.dir()),
            b = -2.0 * dot(ray.dir(), oc),
            c = dot(oc, oc) - radius_ * radius_;

        T discrim = b * b - 4 * a * c;
        return (discrim >= 0);
    }
};

#endif