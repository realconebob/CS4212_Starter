/**
 * @file Renderable.hpp
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief
 * @version 0.1
 * @date 2026-11-13
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_RENDERABLE__26551762324698__
#define CS4212_GRAPHICS_RENDERABLE__26551762324698__

#include "cwrender/Helpers.hpp"
#include "cwrender/Hittables/Cube.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/Hittables/Plane.hpp"
#include "cwrender/Hittables/Sphere.hpp"
#include "cwrender/Hittables/Triangle.hpp"
// #include "cwrender/Scenes/Scene.hpp" // Causes circular include
#include <cstddef>
#include <memory>
#include <stdexcept>

namespace cwrender {

template<Floating T, std::size_t X> class Scene;

template<Floating T, std::size_t X>
struct Renderable: Hittable<T, X>, Material<T, X> {
    protected:
    std::unique_ptr<Hittable<T, X>> collision_;
    std::unique_ptr<Material<T, X>> material_;

    public:
    Renderable& operator=(const Renderable&) = default;
    Renderable& operator=(Renderable&&) = default;
    Renderable(const Renderable&) = default;
    Renderable(Renderable&&) = default;
    Renderable() = delete;
    ~Renderable() = default;

    template <std::derived_from<Hittable<T, X>> H, std::derived_from<Material<T, X>> M> // Prevents slicing
    Renderable(H collision, M material):
        collision_  {std::make_unique<H>(std::move(collision))},
        material_   {std::make_unique<M>(std::move(material))}
        {}

    std::unique_ptr<Renderable<T, X>> clone() {return std::make_unique<Renderable<T, X>>(*this);}



    #pragma region Hittable Overrides
    bool intersect(const RayX<T, X>& ray, HitRecord<T, X>& record, const Interval<T>& range) const override {
        if(!collision_) throw std::logic_error("Collision is not initialized");

        // Now that the hittables don't hold a material we need to set it here if an intersection occured
        bool res = collision_->intersect(ray, record, range);
        if(res) record.set_mat(material_.get());
        return res;
    }

    BoundingBox<T, X> bounds() const override {
        if(!collision_) throw std::logic_error("Collision is not initialized");
        return collision_->bounds();
    }
    #pragma endregion



    #pragma region Material Overrides
    bool scatter(const Scene<T, X>& scene, const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten_out, RayX<T, X>& scattered_out) const override {
        if(!material_) throw std::logic_error("Material is not initialized");
        return material_->scatter(scene, rayin, record, atten_out, scattered_out);
    }

    const VecX<T, X>& reflection() const {
        if(!material_) throw std::logic_error("Material is not initialized");
        return material_->reflection_;
    }

    bool override_atten() const {
        if(!material_) throw std::logic_error("Material is not initialized");
        return material_->override_atten_;
    }
    #pragma endregion



    #pragma region Shape Constructors
    static Renderable<T, 3> Triangle(const VecX<T, 3>& a, const VecX<T, 3>& b, const VecX<T, 3>& c, Material<T, X> mat) {return Renderable<T, 3>{Triangle3<T>{a, b, c},             mat};}
    static Renderable<T, 3> Sphere(const VecX<T, 3>& origin, T radius, Material<T, 3> mat)                              {return Renderable<T, 3>{Sphere3<T>{origin, radius},        mat};}
    static Renderable<T, 3> Plane(const VecX<T, 3>& bottomleft, const VecX<T, 3>& topright, Material<T, 3> mat)         {return Renderable<T, 3>{Plane3<T>{bottomleft, topright},   mat};}
    static Renderable<T, 3> Cube(const VecX<T, 3>& bottomleft, const VecX<T, 3>& topright, Material<T, X> mat)          {return Renderable<T, 3>{Cube3<T>{bottomleft, topright},    mat};}

    #pragma endregion
};

}

#endif