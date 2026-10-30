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

#include "cwrender/BaseTypes/RayX.hpp"
#include "cwrender/BaseTypes/VecX.hpp"
#include "cwrender/BaseTypes/Interval.hpp"

#include <cmath>
#include <cstddef>
#include <memory>

namespace cwrender {

template<Floating T, std::size_t X> class MaterialAny;
template<typename Derived, Floating T, std::size_t X> class Hittable;
template<Floating T, std::size_t X> class HittableAny;
template<Floating T, std::size_t X> class BoundingBox;

/**
 * @brief Record class describing how/if a ray hit some `Hittable` object
 *
 * @tparam T Width of floating point number
 * @tparam X Size of vector
 */
template<Floating T, std::size_t X>
class HitRecord {
    private:
    VecX<T, X> point_, normal_;
    T t_;
    std::shared_ptr<MaterialAny<T, X>> mat_;
    bool front_face_;
    bool override_atten_;

    template<typename D, Floating U, std::size_t Y>
    friend class Hittable;

    public:
    HitRecord(VecX<T, X> point = {}, VecX<T, X> normal = {}, T t = NAN, bool front = true):
        point_{point}, normal_{normal}, t_(t), front_face_(front)
        {}

    const VecX<T, X>& point() const {return point_;}
    const VecX<T, X>& normal() const {return normal_;}
    T t() const {return t_;}
    std::shared_ptr<MaterialAny<T, X>> material() {return mat_;}
    bool front_face() const {return front_face_;}
};

/**
 * @brief Concept requiring the implementation of a member function `bool intersect_impl(const RayX<T, X>&, HitRecord<T, X>&, const Interval<T>&)`
 *
 * @tparam Derived Deriving class
 * @tparam T Size of floating point in RayX
 * @tparam X Number of floating points in RayX
 */
template<typename Derived, typename T, std::size_t X>
concept HittableImpl = Floating<T> && requires(const Derived& d, const RayX<T, X>& ray, HitRecord<T, X>& record, const Interval<T>& range) {
    { d.intersect_impl(ray, record, range) } -> std::convertible_to<bool>;
    { d.bounds_impl() } -> std::convertible_to<BoundingBox<T, X>>;
};

/**
 * @brief CRTP Hittable. Derive from this and implement intersect_impl() to be a generic Hittable
 *
 */
template<typename Derived, Floating T, std::size_t X>
class Hittable {
    private:
    Hittable() = default;
    ~Hittable() = default;
    friend Derived;

    const Derived& self_cast() const {return static_cast<const Derived&>(*this);}
    Derived& self_cast() {return static_cast<Derived&>(*this);}

    protected:

    /**
    * @brief Updates a HitRecord after a successful intersection. Flips the normal
    *        so it always opposes the incoming ray, and records which side was hit.
    *
    * @param record     Record to fill in
    * @param point      Intersection point
    * @param t          Ray parameter at the intersection
    * @param ray        Incident ray (used to determine front/back face)
    * @param out_normal Outward-facing surface normal at the point
    * @param material   Material of the surface that was hit
    */
    static void update(HitRecord<T, X>& record, const VecX<T, X>& point, T t, const RayX<T, X>& ray, const VecX<T, X>& out_normal, std::shared_ptr<MaterialAny<T, X>> material) {
        record.front_face_ = dot(ray.dir(), out_normal) < 0;
        record.normal_ = record.front_face_ ? out_normal : -out_normal;
        record.point_ = point;
        record.t_ = t;
        record.mat_ = std::move(material);
    }

    public:
    template<typename D = Derived>
    bool intersect(const RayX<T, X>& ray, HitRecord<T, X>& record, const Interval<T>& range) const requires (HittableImpl<D, T, X>) {
        return self_cast().intersect_impl(ray, record, range);
    }

    BoundingBox<T, X> bounds() const {
        return self_cast().bounds_impl();
    }

    HittableAny<T, X> any() const {return HittableAny<T, X>(self_cast());}
};

/**
 * @brief Type erasure wrapper for Hittable. Means you can store disperate Hittable types in a single STL
 *
 */
template<Floating T, std::size_t X>
class HittableAny {
    /// "interface" that must be "implemented" by any shape that is hittable
    struct Concept {
        virtual ~Concept() = default;
        virtual bool intersect(const RayX<T, X>&, HitRecord<T, X>&, const Interval<T>&) const = 0;
        virtual BoundingBox<T, X> bounds() const = 0;
    };

    /// Wrapper / adapter. "Stores" original type through template, but is "erased" to a Concept when stored in self_
    template<typename H>
    struct Model final: Concept {
        explicit Model(H h) : obj_(std::move(h)) {}

        bool intersect(const RayX<T, X>& ray, HitRecord<T, X>& record, const Interval<T>& range) const override {
            return obj_.intersect_impl(ray, record, range); // dispatches into the CRTP call, still static from here down
        }
        BoundingBox<T, X> bounds() const override {return obj_.bounds_impl();}

        H obj_;
    };

    std::shared_ptr<const Concept> self_;

public:
    template<typename H>
    requires (!std::same_as<H, HittableAny> && HittableImpl<H, T, X> && std::move_constructible<H>)
    HittableAny(H h): self_(std::make_shared<const Model<H>>(std::move(h))) {}

    bool intersect(const RayX<T, X>& ray, HitRecord<T, X>& record, const Interval<T>& range) const { return self_->intersect(ray, record, range); }
    BoundingBox<T, X> bounds() const {return self_->bounds();}
};

}
#endif