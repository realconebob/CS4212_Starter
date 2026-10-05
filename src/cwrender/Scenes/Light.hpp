/**
 * @file Light.hpp
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Light implementation
 * @version 0.1
 * @date 2026-10-04
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_LIGHT__19222122927224__
#define CS4212_GRAPHICS_LIGHT__19222122927224__

#include "cwrender/BaseTypes/VecX.hpp"
#include "cwrender/Helpers.hpp"
#include <cstddef>
#include <memory>

using namespace cwrender;

namespace cwrender {

template<Floating T, std::size_t X> class LightAny;

template<typename D, Floating T, std::size_t X>
class Light {
    private:
    template<Floating A, std::size_t B>
    friend class LightAny;

    protected:
    VecX<T, X> pos_, color_, intensity_;

    Light(): pos_{}, color_{}, intensity_{} {}
    Light(const Light&) = default;
    Light(Light&&) = default;
    ~Light() = default;

    public:
    VecX<T, X> pos() const {return pos_;}
    VecX<T, X> color() const {return color_;}
    VecX<T, X> intensity() const {return intensity_;}
    std::shared_ptr<LightAny<T, X>> sharedptr() {return std::make_shared<LightAny<T, X>>(std::move(static_cast<const D&>(*this)));}
    LightAny<T, X> any() {return LightAny<T, X>(static_cast<const D&>(*this));}
};

template<Floating T, std::size_t X>
class LightAny {
    struct Concept {
        virtual ~Concept() = default;
        virtual VecX<T, X> pos() const = 0;
        virtual VecX<T, X> color() const = 0;
        virtual VecX<T, X> intensity() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };

    template<typename E>
    struct Emitter: Concept {
        E obj;
        explicit Emitter(E e): obj(std::move(e)) {}

        VecX<T, X> pos() const override {return obj.pos();}
        VecX<T, X> color() const override {return obj.color();}
        VecX<T, X> intensity() const override {return obj.intensity();}
        std::unique_ptr<Concept> clone() const override {return std::make_unique<Emitter>(obj);}
    };

    std::unique_ptr<Concept> self_;

    public:
    template<typename L>
    requires (!std::same_as<std::decay_t<L>, LightAny>)
    LightAny(L light): self_(std::make_unique<Emitter<L>>(std::move(light))) {}
    LightAny(const LightAny& o) : self_(o.self_->clone()) {}
    LightAny(LightAny&&) = default;

    VecX<T, X> pos() const {return self_->pos();}
    VecX<T, X> color() const {return self_->color();}
    VecX<T, X> intensity() const {return self_->intensity();}
};

// I don't actually know if the light is going to do anything other than just provide information about itself. It very well might, and that's what all this is for, but it very well may not. If it doesn't then this is another rep
// of CRTP and Type Erasure under my belt at the very least. I'm thinking that with diffuse lighting textures this will eventually come in handy

}
#endif