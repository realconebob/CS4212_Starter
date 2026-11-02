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
#include <memory>
#include <optional>

namespace cwrender {

template<Floating T, std::size_t X>
class BoundingVolumeHierarchy: public Hittable<BoundingVolumeHierarchy<T, X>, T, X> {
    private:
    struct Node {
        BoundingBox<T, X> box;
        std::shared_ptr<Node> left, right;
        std::optional<HittableAny<T, X>> shape; // only set on leaves

        bool hit(const RayX<T, X>& ray, HitRecord<T, X>& record, const Interval<T>& range) const {
            if (!box.intersect(ray, record, range)) return false;

            if (shape) return shape->intersect(ray, record, range);   // leaf

            bool hit_left  = left && left->hit(ray, record, range);
            // If left hit, anything on the right only counts if it's closer
            Interval<T> right_range(range.min, hit_left ? record.t() : range.max);
            bool hit_right = right && right->hit(ray, record, right_range);

            return hit_left || hit_right;
        }
    };

    std::shared_ptr<Node> root_;

    std::shared_ptr<Node> build(std::vector<HittableAny<T, X>>& objs, std::size_t lo, std::size_t hi) {
        auto node = std::make_shared<Node>();

        // Bounds of everything in [lo, hi)
        BoundingBox<T, X> box = objs[lo].bounds();
        for (std::size_t i = lo + 1; i < hi; i++)
            box = BoundingBox<T, X>(box, objs[i].bounds());   // the union helper
        node->box = box;

        if (hi - lo == 1) {
            node->shape = objs[lo];
            return node;
        }

        // Split along the longest axis of the box
        std::size_t axis = 0;
        T best = box.max()[0] - box.min()[0];
        for (std::size_t i = 1; i < X; i++) {
            T extent = box.max()[i] - box.min()[i];
            if (extent > best) { best = extent; axis = i; }
        }

        // Partition around the median centroid (min+max is the centroid times 2, same ordering)
        std::size_t mid = lo + (hi - lo) / 2;
        std::nth_element(objs.begin() + lo, objs.begin() + mid, objs.begin() + hi,
            [axis](const auto& a, const auto& b) {
                auto ba = a.bounds(), bb = b.bounds();
                return ba.min()[axis] + ba.max()[axis] < bb.min()[axis] + bb.max()[axis];
            });

        node->left  = build(objs, lo, mid);
        node->right = build(objs, mid, hi);
        return node;
    }

    std::vector<HittableAny<T, X>> objects_{};

    public:
    explicit BoundingVolumeHierarchy() {}
    explicit BoundingVolumeHierarchy(std::vector<HittableAny<T, X>> objs) {
        if (!objs.empty()) root_ = build(objs, 0, objs.size());
    }

    void add_front(const HittableAny<T, X>& obj) {objects_.insert(objects_.begin(), obj);}
    void add_back(const HittableAny<T, X>& obj) {objects_.push_back(obj);}

    bool build_from_internal_list() {
        auto res = !objects_.empty();
        if (res) root_ = build(objects_, 0, objects_.size());
        return res;
    }

    bool intersect_impl(const RayX<T, X>& ray, HitRecord<T, X>& record, const Interval<T>& range) const {
        return root_ && root_->hit(ray, record, range);
    }

    BoundingBox<T, X> bounds_impl() const {
        return (root_) ? root_.box : BoundingBox<T, X>{};
    }
};

}
#endif