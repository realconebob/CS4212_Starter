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

template<Floating T, std::size_t X> class Material;
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
    template<Floating t, std::size_t x> friend class Hittable;
    template<Floating t, std::size_t x> friend struct Renderable;

    VecX<T, X> point_{}, normal_{};
    T t_{NAN};
    const Material<T, X>* mat_; // Borrowing pointer, not owning. May get replaced with borrowing pointer to Renderable
    bool front_face_{};
    bool override_atten_{};

    public:
    HitRecord(VecX<T, X> point = {}, VecX<T, X> normal = {}, T t = NAN, bool front = true):
        point_{point}, normal_{normal}, t_(t), front_face_(front)
        {}

    const VecX<T, X>& point() const {return point_;}
    const VecX<T, X>& normal() const {return normal_;}
    T t() const {return t_;}
    const Material<T, X>* material() const {return mat_;}
    bool front_face() const {return front_face_;}

    // fuck it encapsulation is for chumps
    void update(const VecX<T, X>& point, T t, const RayX<T, X>& ray, const VecX<T, X>& out_normal);
    void set_mat(const Material<T, X> * const material);
};

/**
 * @brief Update the contents of a HitRecord
 *
 * @tparam T A floating point type
 * @tparam X The size of the vector
 * @param point The new point of intersection
 * @param t How long the ray had to march to reach the intersection
 * @param ray The ray that created the intersection
 * @param out_normal The outward facing normal
 * @param material The material of the intersected object
 */
template<Floating T, std::size_t X>
void HitRecord<T, X>::update(const VecX<T, X>& point, T t, const RayX<T, X>& ray, const VecX<T, X>& out_normal) {
    front_face_ = dot(ray.dir(), out_normal) < 0;
    normal_ = front_face_ ? out_normal : -out_normal;
    point_ = point;
    t_ = t;
}

template<Floating T, std::size_t X>
void HitRecord<T, X>::set_mat(const Material<T, X> * material) {
    mat_ = material;
}


template<Floating T, std::size_t X>
class Hittable {
    protected:
    Hittable() = default;
    Hittable& operator=(const Hittable&) = default;
    Hittable(const Hittable&) = default;
    Hittable(Hittable&&) = default;

    public:
    virtual ~Hittable() = default;

    virtual bool intersect(const RayX<T, X>& ray, HitRecord<T, X>& record, const Interval<T>& range) const = 0;
    virtual BoundingBox<T, X> bounds() const = 0;
};

}
#endif