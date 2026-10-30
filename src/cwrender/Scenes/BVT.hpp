/**
 * @file BVT.hpp
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Bounding Volume Hierarchy definitions/implementation
 * @version 0.1
 * @date 2026-10-19
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef CS4212_GRAPHICS_BVT__11765199838936__
#define CS4212_GRAPHICS_BVT__11765199838936__

#include "cwrender/Helpers.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include <cstddef>

namespace cwrender {

template<Floating T, std::size_t X>
class BoundingVolumeHierarchy {
    private:
    struct BVTNode: Hittable<BVTNode, T, X> {
        HittableAny<T, X> shape;
        BVTNode* left;
        BVTNode* right;

        bool intersect_impl(const RayX<T, 3>& ray, HitRecord<T, 3>& record, const Interval<T>& range) const {
            bool lefthit = (!left && left->intersect());
            bool rghthit = (!right && right->intersect());

            if(lefthit && rghthit) {} // Inspect left and right for closest
            if(lefthit) {} // Do something for left
            if(rghthit) {} // Do something for right

            return false;
        }

        BoundingBox<T, X> bounds_impl() const {
            throw NotImplemented("TODO: There is actually a correct way to do this but I forgor");
        }
    };

    BVTNode* root_;

    public:
};

}
#endif