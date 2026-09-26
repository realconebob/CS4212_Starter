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

#include "framebuffer.h"
#include "helpers.h"
#include "RayX.h"
#include "vecx.h"

#include <cmath>
#include <cstddef>
#include <vector>

template <typename F, typename T, std::size_t X>
concept RayColorizer = std::is_invocable_r_v<VecX<T, X>, F, const RayX<T, X>&>;

template<Floating T>
class PerspectiveCamera3D {
    public:
    #pragma region Set variables
    VecX<T, 3> lookfrom_, lookat_, up;

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
    PerspectiveCamera3D(
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

    template <RayColorizer<T, 3> Colorizer>
    void rendertobuffer(Framebuffer<T, 3>& fb, Colorizer colorizer) const {
        VecX<T, 3> pixc, rdir;
        RayX<T, 3> r;

        for(int j = 0; j < iheight(); j++) {
            for(int i = 0; i < iwidth; i++) {
                pixc = pix00() + ((i * 1.0) * delta_u()) + ((j * 1.0) * delta_v());
                rdir = pixc - lookfrom_;

                r = RayX<T, 3>{lookfrom_, rdir};
                fb(i, j) = colorizer(r);
            }
        }
    }
};

#endif