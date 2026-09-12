#ifndef CS4212_GRAPHICS_FRAMEBUFFER__110961598615200__
#define CS4212_GRAPHICS_FRAMEBUFFER__110961598615200__

#include "helpers.h"
#include "vecx.h"

#include <concepts>
#include <cstring>
#include <span>
#include <cstddef>
#include <type_traits>

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
        [[nodiscard]] constexpr std::size_t size() const {return width_ * height_;}

        template<typename... Args>
        requires (std::same_as<Args, VecX<T, X>> && ...)
        void clear_to(const Args&... colors) {
            // For each row, calculate how much of each color should be in a given pixel
            // Say with 2 colors, you'd have it look something like: fb[i] = (((size() - i) / size()) * color1 + (i / size()) * color2).unit()
            // Idk what this looks like in pesudocode with 3+ values, but I'm sure it can be generalized

            

            return;
        }

        void clear() {
            if constexpr (std::is_trivially_copyable_v<VecX<T, X>>) {
                std::memset(mem_, 0, width_ * height_ * sizeof(VecX<T, X>));
            } else {
                for(std::size_t i = 0; i < width_ * height_; i++) {
                    mem_[i] = VecX<T, X>{};
                }
            }

            return;
        }

        #pragma endregion
};

#endif