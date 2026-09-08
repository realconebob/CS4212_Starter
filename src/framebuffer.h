#ifndef CS4212_GRAPHICS_FRAMEBUFFER__110961598615200__
#define CS4212_GRAPHICS_FRAMEBUFFER__110961598615200__

#include "vecx.h"
#include <cstring>
#include <span>
#include <array>
#include <cstddef>

template<Floating T, std::size_t X>
class Framebuffer {
    private:
        const std::size_t width_, height_;
        VecX<T, X> mem_[];

    public:
        #pragma region Construtors
        Framebuffer(const std::size_t w, const std::size_t h): width_(w), height_(h) {
            mem_ = new VecX<T, X>[width_ * height_]{};
        }

        Framebuffer() = delete;

        // Making sure a copy can't be silently made
        Framebuffer(const Framebuffer&) = delete;
        Framebuffer(Framebuffer&&) = default;
        #pragma endregion


        #pragma region Destructors
        ~Framebuffer() {
            delete[] mem_;
        }

        #pragma endregion


        #pragma region Operators
        // Accessing a particular pixel is done via `framebuffer(x, y)`
        [[nodiscard]] VecX<T, X>& operator()(std::size_t x, std::size_t y) {return mem_[y * width_ + x];}
        [[nodiscard]] const VecX<T, X>& operator()(std::size_t x, std::size_t y) const {
            return mem_[y * width_ + x];
        }

        // Making sure a copy can't be silently made
        Framebuffer& operator=(const Framebuffer&) = delete;
        Framebuffer& operator=(Framebuffer&&) = default;
        #pragma endregion


        #pragma region Misc
        [[nodiscard]] Framebuffer clone() const {
            Framebuffer copy = Framebuffer<T,X>(width_, height_);
            std::memmove(copy.mem_, mem_, sizeof(VecX<T, X>) * width_ * height_);
            return copy;
        }

        [[nodiscard]] std::span<VecX<T, X>> as_span() {return {mem_.data(), mem_.size()};}
        [[nodiscard]] std::span<const VecX<T, X>> as_span() const {return {mem_.data(), mem_.size()};}

        [[nodiscard]] constexpr std::size_t width() const {return width_;}
        [[nodiscard]] constexpr std::size_t height() const {return height_;}
        #pragma endregion
};

#endif