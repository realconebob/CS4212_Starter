#ifndef CS4212_GRAPHICS_FRAMEBUFFER__110961598615200__
#define CS4212_GRAPHICS_FRAMEBUFFER__110961598615200__

#include "helpers.h"
#include "vecx.h"

#include <concepts>
#include <cstring>
#include <cstddef>
#include <type_traits>

template<Floating T, std::size_t X>
class Framebuffer {
    private:
        const std::size_t width_, height_;
        VecX<T, X>* mem_; // spooky but it should be fine for now. Maybe rework to be a std::vector eventually

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

        /*
        [[nodiscard]] std::span<VecX<T, X>> as_span() {return {mem_.data(), mem_.size()};}
        [[nodiscard]] std::span<const VecX<T, X>> as_span() const {return {mem_.data(), mem_.size()};}
        */

        [[nodiscard]] constexpr std::size_t width() const {return width_;}
        [[nodiscard]] constexpr std::size_t height() const {return height_;}
        [[nodiscard]] constexpr std::size_t size() const {return width_ * height_;}

        template<typename... Args>
        requires (sizeof...(Args) >= 1) && (std::same_as<Args, VecX<T, X>> && ...)
        void clear_to(const Args&... colors) {
            const std::size_t argnum = sizeof...(Args);
            const VecX<T, X> carr[argnum] = {colors...};


            // currently blowing up my code for some reason
            // if(argnum == 1) {
            //     std::fill(std::begin(mem_), std::end(mem_), carr[0]);
            //     return;
            // }
            if(argnum == 1) {
                for(std::size_t i = 0; i < size(); i++) {
                    mem_[i] = carr[0];
                }
            }

            VecX<T, X> curpix;
            std::size_t low;
            std::size_t mid;
            std::size_t high;

            const std::size_t memsize = size();
            const T invargnum = 1 / (argnum - 1);

            for(std::size_t i = 0; i < memsize; i++) {
                curpix = VecX<T, X>{};

                // Calculate the amount of color each color should contribute to the current pixel, then write it to the buffer
                for(std::size_t ci = 0; ci < argnum; ci++) {
                    low = ((ci * memsize - memsize) * invargnum);
                    high = ((ci * memsize + memsize) * invargnum);
                    mid = (low + high) * (1/2);

                    curpix += 
                        static_cast<int>((i >= low) && (i <= high)) // Determine whether the color should be present for this particular pixel. Inside range = 1, outside range = 0
                        * (1 - relative_diff(i, mid))
                        * carr[ci];
                }
                mem_[i] = unit(curpix); // May not need to normalize but unsure yet. Need to get print to png working to test
            }

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