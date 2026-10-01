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
template<Floating T, std::size_t X> class Lambertian;
template<typename Derived, Floating T, std::size_t X> class Hittable;

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
    std::shared_ptr<MaterialAny<T, X>> mat_; // spooky (MaterialAny instead of Material)
    bool front_face_;

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
 * @brief Concept requiring the implementation of a member function `bool _intersect(const RayX<T, X>& ray)`
 *
 * @tparam Derived Deriving class
 * @tparam T Size of floating point in RayX
 * @tparam X Number of floating points in RayX
 */
template<typename Derived, typename T, std::size_t X>
concept HittableImpl = Floating<T> && requires(const Derived& d, const RayX<T, X>& ray, HitRecord<T, X>& record, const Interval<T>& range) {
    { d._intersect(ray, record, range) } -> std::convertible_to<bool>;
};

template<typename Derived, Floating T, std::size_t X>
/**
 * @brief CRTP Hittable. Derive from this and implement _intersect() to be a generic Hittable
 *
 */
class Hittable {
    protected:
    ~Hittable() = default;

    /**
     * @brief Friend function of HitRecord. Used to update the contents of a record when checking for intersects
     *
     * @param record
     * @param point
     * @param normal
     * @param t
     */
    static void update(HitRecord<T, X>& record, const VecX<T, X>& point, T t, const RayX<T, X>& ray, const VecX<T, X>& out_normal, std::shared_ptr<MaterialAny<T, X>> material) {
        record.front_face_ = dot(ray.dir(), out_normal) < 0;
        record.normal_ = record.front_face_ ? out_normal : -out_normal;
        record.point_ = point;
        record.t_ = t;
        record.mat_ = material;
    }

    public:
    template<typename D = Derived>
    bool intersect(const RayX<T, X>& ray, HitRecord<T, X>& record, const Interval<T>& range) const requires (HittableImpl<D, T, X>) {
        return static_cast<const D*>(this)->_intersect(ray, record, range);
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
        virtual bool intersect(const RayX<T, X>&, HitRecord<T, X>&, const Interval<T>&) const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };

    /// Wrapper / adapter. "Stores" original type through template, but is "erased" to a Concept when stored in self_
    template<typename H>
    struct Model: Concept {
        H obj;
        explicit Model(H h) : obj(std::move(h)) {}
        bool intersect(const RayX<T, X>& ray, HitRecord<T, X>& record, const Interval<T>& range) const override {
            return obj.intersect(ray, record, range); // dispatches into the CRTP call, still static from here down
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

    bool intersect(const RayX<T, X>& ray, HitRecord<T, X>& record, const Interval<T>& range) const { return self_->intersect(ray, record, range); }
};

}
#endif