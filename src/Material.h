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

#include "Helpers.h"
#include "Hittable.h"
#include "VecX.h"

#include <cstddef>
#include <memory>
#include <utility>
#include <concepts>

template<typename Derived, typename T, std::size_t X>
concept MaterialImpl = Floating<T> && requires(const Derived& d, const HitRecord<T, X>& record) {
    { d._shade(record) } -> std::convertible_to<VecX<T, X>>;
};

template<typename M, typename T, std::size_t X>
concept Shadeable = requires(const M& m, const HitRecord<T, X>& record) {
    { m.raycolor(record) } -> std::convertible_to<VecX<T, X>>;
};

template<typename Derived, Floating T, std::size_t X>
class Material {
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

    Material(): reflection_{VecX<T, X>::Ones()} {}
    Material(const Material&) = default;
    Material(Material&&) = default;
    ~Material() = default;

    public:
    VecX<T, X> raycolor(const HitRecord<T, X>& record) const
    requires (MaterialImpl<Derived, T, X>){
        return static_cast<const Derived&>(*this)->_shade(record);
    }

    const VecX<T, X>& reflection() const {return reflection_;}
    VecX<T, X> absorption() const {return (VecX<T, X>::Ones() - reflection_);}
};

template<Floating T, std::size_t X>
class MaterialAny {
    struct Concept {
        virtual ~Concept() = default;
        virtual VecX<T, X> raycolor(const HitRecord<T, X>&) const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };

    template<typename S>
    struct Shader: Concept {
        S obj;
        explicit Shader(S s) : obj(std::move(s)) {}
        VecX<T, X> raycolor(const HitRecord<T, X>& record) const override {
            return obj.raycolor(record);
        }
        std::unique_ptr<Concept> clone() const override {
            return std::make_unique<Shader>(obj);
        }
    };

    std::unique_ptr<Concept> self_;

    public:
    template<typename M>
    requires (MaterialImpl<M, T, X>) && (Shadeable<std::decay_t<M>, T, X>) && (!std::same_as<std::decay_t<M>, MaterialAny>)
    MaterialAny(M material): self_(std::make_unique<Shader<M>>(std::move(material))) {}

    MaterialAny(const MaterialAny& o) : self_(o.self_->clone()) {}
    MaterialAny(MaterialAny&&) = default;

    VecX<T, X> raycolor(const HitRecord<T, X>& record) const {
        return self_->raycolor(record);
    }
};

#endif