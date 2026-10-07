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
#include <iostream>
#include <ostream>
#include <vector>

namespace cwrender {

template <typename F, typename T, std::size_t X>
concept RayColorizer = std::is_invocable_r_v<VecX<T, X>, F, const RayX<T, X>&>;

template<Floating T>
class PerspectiveCamera3 {
    public:
    #pragma region Set variables
    VecX<T, 3> lookfrom_, lookat_, up;
    int samplegrid_ = 8;

    T aspectratio, vfov;
    int iwidth;
    #pragma endregion

    private:
    #pragma region Calculated variables
    int iheight() const {
        auto tmp = int(iwidth / aspectratio);
        return (tmp < 1) ? 1 : tmp;
    }

    std::vector<T> viewport_dims() const {
        auto focallen = (lookfrom_ - lookat_).length();
        auto theta = degtorad(vfov);
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

    VecX<T, 3> center() const {return lookfrom_;}
    VecX<T, 3> pix00() const {
        auto focallen = viewport_dims()[0];
        auto upper_left = center() - (focallen * w()) - viewport_u()/2.0 - viewport_v()/2.0;
        return upper_left + 0.5 * (delta_u() + delta_v());
    }

    VecX<T, 3> delta_u() const {return viewport_u() / T(iwidth);}
    VecX<T, 3> delta_v() const {return viewport_v() / T(iheight());}

    VecX<T, 3> u() const {return unit(cross(up, w()));}
    VecX<T, 3> v() const {return cross(w(), u());}
    VecX<T, 3> w() const {return unit(lookfrom_ - lookat_);}
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
        lookfrom_{lookfrom}, lookat_{lookat}, up{vup},
        aspectratio{aspect}, vfov{fov},
        iwidth{imgwidth}
        {}

    int get_iwidth() const {return iwidth;}
    int get_iheight() const {return iheight();}

    RayX<T, 3> getray(int i, int j) const {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        auto offset = sample_square();
        auto pixc = pix00() + ((i + offset[0]) * delta_u()) + ((j + offset[1]) * delta_v());
        auto rdir = pixc - lookfrom_;
        return RayX<T, 3>{lookfrom_, rdir};
    }

    VecX<T, 3> sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return VecX<T, 3>(zorandr<T>(-0.5, 0.5), zorandr<T>(-0.5, 0.5), 0);
    }

    VecX<T, 3> raycolor(const Ray3D& ray, const HittableAny<T, 3>& world, const std::vector<LightAny<T, 3>>& lights, HitRecord<T, 3>& record, const Interval<T>& range, int depth) {
        if(depth <= 0) return VecX<T, 3>{};

        if(world.intersect(ray, record, range)) {
            RayX<T, 3> scattered;
            VecX<T, 3> atten{};
            if(record.material()->scatter(ray, record, atten, scattered, lights))
                return (record.material()->override_atten())
                    ? atten
                    : atten * raycolor(scattered, world, lights, record, range, depth - 1);

            return VecX<T, 3>{};
        }

        VecX<T, 3> udir = unit(ray.dir());
        auto a = T(0.5) * (udir[1] + T(1.0));
        return (T(1.0) - a) * VecX<T, 3>::Ones() + a * VecX<T, 3>(0.5, 0.7, 1.0);
    }

    template <RayColorizer<T, 3> Colorizer>
    void rendertobuffer(Framebuffer<T, 3>& fb, Colorizer colorizer) const {
        VecX<T, 3> pixc, rdir;
        RayX<T, 3> r;

        const auto
            pixtl = pix00(),
            du = delta_u(),
            dv = delta_v();

        const T pss = T(1.0) / (samplegrid_*samplegrid_);
        for(int j = 0; j < iheight(); j++) {
            std::clog << "\rScanlines remaining: " << (iheight() - j) << " (" << int(j*100/iheight()) << "%)      "  << std::flush;
            for(int i = 0; i < iwidth; i++) {
                auto color = VecX<T, 3>::Zeros();
                // Random sampling
                // for(int sample = 0; sample < samples_; sample++) {
                //     color += colorizer(getray(i, j));
                // }
                // fb(i, j) = color * pss;

                // Jitter sampling
                for(int a = 0; a < samplegrid_ - 1; a++) {
                for(int b = 0; b < samplegrid_ - 1; b++) {
                    r = getray(i + (a + zorandr<T>(-0.5, 0.5))/samplegrid_, j + (b + zorandr<T>(-0.5, 0.5))/samplegrid_);
                    color += colorizer(r);
                }}
                fb(i, j) = color * pss;
            }
        }
        std::clog << "Done                                                                             \n" << std::flush;
    }
};

}
#endif