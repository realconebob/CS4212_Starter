/**
 * @file Hittable.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief CRTP interface for something that can intersect a ray
 * @version 0.1
 * @date 2026-09-26
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_HITTABLE__90832129712705__
#define CS4212_GRAPHICS_HITTABLE__90832129712705__

#include "RayX.h"
#include <memory>

/**
 * @brief Concept requiring the implementation of a member function `bool _intersect(const RayX<T, X>& ray)`
 *
 * @tparam Derived Deriving class
 * @tparam T Size of floating point in RayX
 * @tparam X Number of floating points in RayX
 */
template<typename Derived, typename T, std::size_t X>
concept HittableImpl = Floating<T> && requires(const Derived& d, const RayX<T, X>& ray) {
    { d._intersect(ray) } -> std::convertible_to<bool>;
};

template<typename Derived, Floating T, std::size_t X>
/**
 * @brief CRTP Hittable. Derive from this and implement _intersect() to be a generic Hittable
 *
 */
class Hittable {
    protected:
    ~Hittable() = default;

    public:
    template<typename D = Derived>
    bool intersect(const RayX<T, X>& ray) const requires (HittableImpl<D, T, X>) {
        return static_cast<const D*>(this)->_intersect(ray);
    };
};


template<Floating T, std::size_t X>
/**
 * @brief Type erasure wrapper for Hittable. Means you can store disperate Hittable types in a single STL
 *
 */
class HittableAny {
    /// "interface" that must be "implemented" by any shape that is hittable
    struct Concept {
        virtual ~Concept() = default;
        virtual bool intersect(const RayX<T, X>&) const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };

    /// Wrapper / adapter. "Stores" original type through template, but is "erased" to a Concept when stored in self_
    template<typename H>
    struct Model: Concept {
        H obj;
        explicit Model(H h) : obj(std::move(h)) {}
        bool intersect(const RayX<T, X>& ray) const override {
            return obj.intersect(ray); // dispatches into the CRTP call, still static from here down
        }
        std::unique_ptr<Concept> clone() const override {
            return std::make_unique<Model>(obj);
        }
    };

    std::unique_ptr<Concept> self_;

public:
    template<typename H>
    requires (HittableImpl<H, T, X>) // Technically should be a concept that checks for intersect instead of _intersect, but I'm only going to be using my own CRTP-style Hittables anyway, so this shouldn't be a problem
    HittableAny(H h) : self_(std::make_unique<Model<H>>(std::move(h))) {}

    HittableAny(const HittableAny& o) : self_(o.self_->clone()) {}
    HittableAny(HittableAny&&) = default;

    bool intersect(const RayX<T, X>& ray) const { return self_->intersect(ray); }
};

#endif