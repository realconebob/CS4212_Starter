/**
 * @file Material.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Material implementation
 * @version 0.1
 * @date 2026-09-29
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_MATERIAL__3962847032450__
#define CS4212_GRAPHICS_MATERIAL__3962847032450__

#include "cwrender/Helpers.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/BaseTypes/VecX.hpp"
#include "cwrender/Scenes/Light.hpp"

#include <cstddef>
#include <memory>
#include <utility>
#include <concepts>
#include <vector>

namespace cwrender {

template<typename Derived, typename T, std::size_t X>
concept MaterialImpl = Floating<T> && requires(const Derived& d, const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered, const std::vector<LightAny<T, X>>& lights) {
    { d._scatter(rayin, record, atten, scattered, lights) } -> std::convertible_to<bool>;
};

template<typename M, typename T, std::size_t X>
concept Shadeable = requires(const M& m, const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered, const std::vector<LightAny<T, X>>& lights) {
    { m.scatter(rayin, record, atten, scattered, lights) } -> std::convertible_to<bool>;
};


template<Floating T, std::size_t X> class MaterialAny;


template<typename Derived, Floating T, std::size_t X>
class Material {
    private:
    template<Floating U, std::size_t V>
    friend class MaterialAny;

    protected:

    /**
    @brief The amount of color that should be reflected, channelwise. `VecX<T, 3>{1, 1, 1}` would
    reflect all light, `VecX<T, 3>{0, 0, 0}` would absorb all light, `VecX<T, 3>{1, 0, 0}` would absorb
    all non-red light

    @note A green reflecting sphere being hit with no green light will look black. A white reflecting
    sphere being hit with only green light will look green. reflection is not color, it modulates what
    channels of light get reflected
    */
    VecX<T, X> reflection_;

    /**
     * @brief Whether or not to override a black color with the attenuation of the shader on raycolor
     *
     */
    bool override_atten_;

    Material(): reflection_{VecX<T, X>::Ones()}, override_atten_(false) {}
    Material(const Material&) = default;
    Material(Material&&) = default;
    ~Material() = default;

    public:
    bool scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered, const std::vector<LightAny<T, X>>& lights) const
    requires (MaterialImpl<Derived, T, X>){
        return static_cast<const Derived&>(*this)->_scatter(rayin, record, atten, scattered, lights);
    }

    const VecX<T, X>& reflection() const {return reflection_;}
    VecX<T, X> absorption() const {return (VecX<T, X>::Ones() - reflection_);}
    bool override_atten() const {return override_atten_;}
    // std::shared_ptr<MaterialAny<T, X>> sharedptr() {return std::make_shared<MaterialAny<T, X>>(std::move(static_cast<const Derived&>(*this)));}

    MaterialAny<T, X> any() {return MaterialAny<T, X>(static_cast<const Derived&>(*this));}
    std::shared_ptr<MaterialAny<T, X>> sharedptr() {return std::make_shared<MaterialAny<T, X>>(any());}
};

template<Floating T, std::size_t X>
class MaterialAny {
    struct Concept {
        virtual ~Concept() = default;
        virtual bool scatter(const RayX<T, X>&, const HitRecord<T, X>&, VecX<T, X>&, RayX<T, X>&, const std::vector<LightAny<T, X>>&) const = 0;
        virtual bool override_atten() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };

    template<typename S>
    struct Shader: Concept {
        S obj;
        explicit Shader(S s) : obj(std::move(s)) {}
        bool scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered, const std::vector<LightAny<T, X>>& lights) const override {
            return obj.scatter(rayin, record, atten, scattered, lights);
        }
        bool override_atten() const override {return obj.override_atten_;}
        std::unique_ptr<Concept> clone() const override {
            return std::make_unique<Shader>(obj);
        }
    };

    std::unique_ptr<Concept> self_;

    public:
    template<typename M>
    requires Shadeable<std::decay_t<M>, T, X> && (!std::same_as<std::decay_t<M>, MaterialAny>)
    MaterialAny(M material): self_(std::make_unique<Shader<M>>(std::move(material))) {}

    MaterialAny(const MaterialAny& o) : self_(o.self_->clone()) {}
    MaterialAny(MaterialAny&&) = default;

    bool scatter(const RayX<T, X>& rayin, const HitRecord<T, X>& record, VecX<T, X>& atten, RayX<T, X>& scattered, const std::vector<LightAny<T, X>>& lights) const {
        return self_->scatter(rayin, record, atten, scattered, lights);
    }
    bool override_atten() const {return self_->override_atten();}
};

}
#endif