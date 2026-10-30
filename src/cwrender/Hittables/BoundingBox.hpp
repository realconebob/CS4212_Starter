/**
 * @file BoundingBox.hpp
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Bounding Box definition
 * @version 0.1
 * @date 2026-10-30
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS__BOUNDING_BOX__2572624085__
#define CS4212_GRAPHICS__BOUNDING_BOX__2572624085__

#include "cwrender/Helpers.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include <cstddef>

namespace cwrender {

template<Floating T, std::size_t X>
class BoundingBox: public Hittable<BoundingBox<T, X>, T, X> {
    private:
    VecX<T, X>
        minpoint_ = VecX<T, X>::Zeros(),
        maxpoint_ = VecX<T, X>::Zeros();

    public:
    BoundingBox() {}
    BoundingBox(VecX<T, X> min, VecX<T, X> max):
        minpoint_(min), maxpoint_(max)
        {}

    bool intersect_impl(const RayX<T, X>& ray, HitRecord<T, X>& record, const Interval<T>& range) const {
        T near = range.min, far = range.max;

        for(std::size_t i = 0; i < X; i++) {
            T amin = ((ray.dir()[i] >= 0 ? minpoint_[i] : maxpoint_[i]) - ray.origin()[i]) / ray.dir()[i];
            T amax = ((ray.dir()[i] >= 0 ? maxpoint_[i] : minpoint_[i]) - ray.origin()[i]) / ray.dir()[i];

            if (amin > near) near = amin;
            if (amax < far) far = amax;

            if (far <= near) return false;
        }
        return true;
    }

    BoundingBox<T, X> bounds_impl() const {
        return *this;
    }
};

}
#endif