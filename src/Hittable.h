#ifndef CS4212_GRAPHICS_HITTABLE__90832129712705__
#define CS4212_GRAPHICS_HITTABLE__90832129712705__

#include "RayX.h"

template<typename Derived, typename T, std::size_t X>
concept HittableImpl = Floating<T> && requires(const Derived& d, const RayX<T, X>& ray) {
    { d._intersect(ray) } -> std::convertible_to<bool>;
};

template<typename Derived, Floating T, std::size_t X>
class Hittable {
    protected:
    ~Hittable() = default;
    
    public:
    bool intersect(const RayX<T, X>& ray) const requires (HittableImpl<Derived, T, X>) {
        return static_cast<const Derived*>(this)->_intersect(ray);
    };
};

#endif