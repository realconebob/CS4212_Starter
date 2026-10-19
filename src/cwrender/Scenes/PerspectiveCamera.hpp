/**
 * @file PerspectiveCamera3D.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief A camera with a focal length
 * @version 0.1
 * @date 2026-09-26
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_CAMERA3D__78361307319100__
#define CS4212_GRAPHICS_CAMERA3D__78361307319100__

#include "cwrender/Scenes/Framebuffer.hpp"
#include "cwrender/Helpers.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/BaseTypes/Interval.hpp"
#include "cwrender/BaseTypes/RayX.hpp"
#include "cwrender/BaseTypes/VecX.hpp"
#include "cwrender/Scenes/Light.hpp"

#include <cmath>
#include <cstddef>
#include <vector>

namespace cwrender {

template <typename F, typename T, std::size_t X>
concept RayColorizer = std::is_invocable_r_v<VecX<T, X>, F, const RayX<T, X>&>;

template<Floating T>
class PerspectiveCamera3 {
    public:
    #pragma region Set variables
    VecX<T, 3> lookfrom, lookat, up;
    int samplegrid = 8; // 64 samples per pixel

    T aspectratio;
    int iwidth;
    #pragma endregion

    private:
    T vfov_;
    #pragma region Calculated variables
    int iheight() const {
        auto tmp = int(iwidth / aspectratio);
        return (tmp < 1) ? 1 : tmp;
    }

    std::vector<T> viewport_dims() const {
        auto focallen = (lookfrom - lookat).length();
        auto theta = degtorad(vfov_);
        auto h = std::tan(theta/2.0);
        auto viewport_h = 2 * h * focallen;
        auto viewport_w = viewport_h * (double(iwidth) / iheight());

        std::vector<T> res = {focallen, viewport_w, viewport_h};
        return res;
    }
    VecX<T, 3> viewport_u() const {
        auto viewport_w = viewport_dims()[1];
        return viewport_w * u();
    }
    VecX<T, 3> viewport_v() const {
        auto viewport_h = viewport_dims()[2];
        return viewport_h * -v();
    }

    VecX<T, 3> center() const {return lookfrom;}
    VecX<T, 3> pix00() const {
        auto focallen = viewport_dims()[0];
        auto upper_left = center() - (focallen * w()) - viewport_u()/2.0 - viewport_v()/2.0;
        return upper_left + 0.5 * (delta_u() + delta_v());
    }

    VecX<T, 3> delta_u() const {return viewport_u() / T(iwidth);}
    VecX<T, 3> delta_v() const {return viewport_v() / T(iheight());}

    VecX<T, 3> u() const {return unit(cross(up, w()));}
    VecX<T, 3> v() const {return cross(w(), u());}
    VecX<T, 3> w() const {return unit(lookfrom - lookat);}
    #pragma endregion

    public:
    PerspectiveCamera3(
            VecX<T, 3> lookfrom = VecX<T, 3>{0, 0, 0},
            VecX<T, 3> lookat = VecX<T, 3>{0, 0, -1},
            VecX<T, 3> vup = VecX<T, 3>{0, 1, 0},
            T aspect = 16.0/9,
            T fov = 90,
            int imgwidth = 200
        ):
        lookfrom{lookfrom}, lookat{lookat}, up{vup},
        aspectratio{aspect}, vfov_{fov},
        iwidth{imgwidth}
        {}

    int get_iwidth() const {return iwidth;}
    int get_iheight() const {return iheight();}

    /// vfov adaptor. sensor_h is measured in milimeters
    void set_focal(T focal, T sensor_h = T(24)) {
        vfov_ = radtodeg(2 * std::atan(sensor_h / (2 * focal)));
    }

    void set_fov(T vfov) {
        vfov_ = vfov;
    }

    RayX<T, 3> getray(int i, int j, int a = 0, int b = 0, int n = 1) const {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        T offx = (a + zorandr<T>(0, 1)) / n - T(0.5);
        T offy = (b + zorandr<T>(0, 1)) / n - T(0.5);
        auto pixc = pix00() + ((i + offx) * delta_u()) + ((j + offy) * delta_v());
        return RayX<T, 3>{lookfrom, pixc - lookfrom};
    }

    VecX<T, 3> sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return VecX<T, 3>(zorandr<T>(-0.5, 0.5), zorandr<T>(-0.5, 0.5), 0);
    }

    VecX<T, 3> raycolor(const Ray3D& ray, const HittableAny<T, 3>& world, const std::vector<LightAny<T, 3>>& lights, HitRecord<T, 3>& record, const Interval<T>& range, int depth) {
        if(depth <= 0) return VecX<T, 3>{};

        thread_local HitRecord<T, 3> real_record{};

        if(world.intersect(ray, real_record, range)) {
            RayX<T, 3> scattered;
            VecX<T, 3> atten{};
            if(real_record.material()->scatter(ray, real_record, atten, scattered, lights))
                return (real_record.material()->override_atten())
                    ? atten
                    : atten * raycolor(scattered, world, lights, real_record, range, depth - 1);

            return VecX<T, 3>{};
        }

        // This should get extracted out into a background color / colorizer
        VecX<T, 3> udir = unit(ray.dir());
        auto a = T(0.5) * (udir[1] + T(1.0));
        return (T(1.0) - a) * VecX<T, 3>::Ones() + a * VecX<T, 3>(0.5, 0.7, 1.0);
    }

    template <RayColorizer<T, 3> Colorizer>
    void rendertobuffer(Framebuffer<T, 3>& fb, Colorizer colorizer) const {
        const auto
            pixtl = pix00(),
            du = delta_u(),
            dv = delta_v();

        const T pss = T(1.0) / (samplegrid*samplegrid);

        #pragma omp parallel for schedule(dynamic)
        for(int j = 0; j < iheight(); j++) {
            for(int i = 0; i < iwidth; i++) {
                auto color = VecX<T, 3>::Zeros();
                for(int a = 0; a < samplegrid; a++) {
                for(int b = 0; b < samplegrid; b++) {
                    const auto r = getray(i, j, a, b, samplegrid);
                    color += colorizer(r);
                }}
                fb(i, j) = color * pss;
            }
        }
    }
};

}
#endif