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
class BoundingBox: public Hittable<T, X> {
    private:
    template<Floating t, std::size_t x> friend class Hittable;

    VecX<T, X>
        minpoint_ = VecX<T, X>::Zeros(),
        maxpoint_ = VecX<T, X>::Zeros();

    public:
    explicit BoundingBox() {}
    explicit BoundingBox(VecX<T, X> min, VecX<T, X> max):
        minpoint_(min), maxpoint_(max)
        {}

    BoundingBox(const BoundingBox<T, X>& a, const BoundingBox<T, X>& b) {
        for (std::size_t i = 0; i < X; i++) {
            minpoint_[i] = std::min(a.minpoint_[i], b.minpoint_[i]);
            maxpoint_[i] = std::max(a.maxpoint_[i], b.maxpoint_[i]);
        }
    }

    const VecX<T, X>& min() const {return minpoint_;}
    const VecX<T, X>& max() const {return maxpoint_;}

    bool intersect(const RayX<T, X>& ray, HitRecord<T, X>& record, const Interval<T>& range) const override {
        T near = range.min, far = range.max;

        for (std::size_t i = 0; i < X; i++) {
            const T inv_d = T(1) / ray.dir()[i];
            T t0 = (minpoint_[i] - ray.origin()[i]) * inv_d;
            T t1 = (maxpoint_[i] - ray.origin()[i]) * inv_d;
            if (inv_d < T(0)) std::swap(t0, t1);

            if (t0 > near) near = t0;
            if (t1 < far)  far  = t1;

            if (far < near) return false;
        }
        return true;
    }

    BoundingBox<T, X> bounds() const override {
        return *this;
    }
};

}
#endif