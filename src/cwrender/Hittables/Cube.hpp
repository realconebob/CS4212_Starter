/**
 * @file Cube.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Cube implementation
 * @version 0.1
 * @date 2026-09-28
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_CUBE__28344179127720__
#define CS4212_GRAPHICS_CUBE__28344179127720__

#include "cwrender/Helpers.hpp"
#include "cwrender/Hittables/BoundingBox.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/Hittables/Plane.hpp"

namespace cwrender {

template<Floating T>
class Cube3: public Hittable<T, 3> {
    private:
    template<Floating t, std::size_t x> friend class Hittable;

    Plane3<T> planes_[6]{};
    VecX<T, 3> bottomleft_{}, topright_{};

    public:
    Cube3(VecX<T, 3> bottomleft, VecX<T, 3> topright):
        bottomleft_(bottomleft), topright_(topright),
        planes_{ // This is gross
            Plane3<T>::empty(), Plane3<T>::empty(), Plane3<T>::empty(),
            Plane3<T>::empty(), Plane3<T>::empty(), Plane3<T>::empty()
        } {
        throw NotImplemented();
    }

    bool intersect(const RayX<T, 3>& ray, HitRecord<T, 3>& record, const Interval<T>& range) const override {
        for(int i = 0; i < 6; i++) if(planes_[i].intersect(ray, record, range)) return true;
        return false;
    }

    BoundingBox<T, 3> bounds() const override {
        return BoundingBox<T, 3>{bottomleft_, topright_};
    }

};

}
#endif