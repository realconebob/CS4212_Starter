#ifndef CS4212_GRAPHICS_CAMERA3D__78361307319100__
#define CS4212_GRAPHICS_CAMERA3D__78361307319100__

#include "framebuffer.h"
#include "helpers.h"
#include "RayX.h"
#include "vecx.h"

#include <cmath>
#include <cstddef>

template <typename F, typename T, std::size_t X>
concept RayColorizer = std::is_invocable_r_v<VecX<T, X>, F, const RayX<T, X>&>;

template<Floating T>
class Camera3D {
    private:
    static const int VECD = 3;
    double samplerate_;

    protected:
    #pragma region Variables
    VecX<T, VECD> lookfrom_, lookat_;

    int // Will be useful layer for antialiasing and bouncing light
        samples_,
        max_depth_;

    double
        focallen_,
        vwidth_,
        aspect_,
        vfov_;

    int
        iwidth_,
        iheight_;

    VecX<T, VECD> u_, v_, w_, up_;
    VecX<T, VECD> vu_, vv_;
    #pragma endregion

    #pragma region Protected Helper Functions
    const VecX<T, VECD> deltaU() const {return u_ / T(iwidth_);}
    const VecX<T, VECD> deltaV() const {return v_ / T(iheight_);}
    const VecX<T, VECD> upperleft() const {return lookfrom_ - (VecX<T, VECD>{0, 0, focallen_} * w_) - (vu_/2.0) - (vv_/2.0);}
    const VecX<T, VECD> pix00loc() const {return upperleft() + 0.5 * (deltaU() + deltaV());}
    inline const int iheight() {
        iheight_ = floor(int(iwidth_ / aspect_), 1);
        return iheight_;
    }

    #pragma endregion

    public:
    #pragma region Constructors
    /**
     * @brief Camera3D builder. Carries default values that make a valid camera, which can be changed
     *
     */
    class Builder {
        public:
        VecX<T, VECD>
            lookfrom = VecX<T, VECD>{},
            lookat = VecX<T, VECD>{0, 0, -1},
            up = VecX<T, VECD>{0, 1, 0};

        int samples = 10, max_depth = 10;
        double aspect = (16.0 / 9.0), vfov = 90.0, iwidth = 200;

        Builder& set_lookfrom(VecX<T, VECD> lf) {lookfrom = lf; return *this;}
        Builder& set_lookat(VecX<T, VECD> la) {lookat = la; return *this;}
        Builder& set_up(VecX<T, VECD> u) {up = u; return *this;}

        Builder& set_samples(int s) {samples = s; return *this;}
        Builder& set_depth(int d) {max_depth = d; return *this;}

        Builder& set_aspect(double a)   {aspect = a; return *this;}
        Builder& set_vfov(double f)     {vfov = f; return *this;}
        Builder& set_iwidth(double w)   {iwidth = w; return *this;}

        private:
        friend Camera3D<T>;

        double imgheight() const {return floor(int(iwidth / aspect), 1);}
        double pixelsamplescale() const {return 1.0 / samples;}
        double focallen() const {return (lookfrom - lookat).length();}
        double vheight() const {
            auto theta = degtorad(vfov);
            auto h = std::tan(theta/2.0);
            return 2 * h * focallen();
        }
        double vwidth() const {return vheight() * double(iwidth / imgheight());}
        VecX<T, VECD> vec_u() const {return unit(cross(up, vec_w()));}
        VecX<T, VECD> vec_v() const {return unit(cross(vec_w(), vec_u()));}
        VecX<T, VECD> vec_w() const {return unit(lookfrom - lookat);}
        VecX<T, VECD> vec_vv() const {return vwidth() * vec_u();}
        VecX<T, VECD> vec_vu() const {return vheight() * -vec_v();}
    };
    explicit Camera3D(Builder& b = Builder{}):
        samples_{b.samples}, max_depth_{b.max_depth}, samplerate_{b.pixelsamplescale()},
        focallen_{b.focallen()}, aspect_{b.aspect}, vfov_{b.vfov}, vwidth_{b.vwidth()},
        iwidth_{int(b.iwidth)}, iheight_{int(b.imgheight())},
        u_{b.vec_u()}, v_{b.vec_v()}, w_{b.vec_w()},
        vv_{b.vec_vv()}, vu_{b.vec_vu()},
        lookfrom_{b.lookfrom}, lookat_(b.lookat), up_(b.up)
        {}

    #pragma endregion

    #pragma region Misc
    const int get_imagewidth() const {return iwidth_;}
    const int get_imageheight() const {return iheight_;}

    template <RayColorizer<T, VECD> Colorizer>
    void rendertobuffer(Framebuffer<T, VECD>& fb, Colorizer colorizer) const {
        if constexpr (std::is_pointer_v<Colorizer>) {
            assert(colorizer != nullptr && "raycolorizer must not be null");
        }

        const VecX<T, VECD>
            pix0 {pix00loc()},
            du {deltaU()},
            dv {deltaV()};

        VecX<T, VECD>
            pixc,
            rdir,
            color;

        RayX<T, VECD> r;

        for(int j = 0; j < iheight_; j++) {
            for(int i = 0; i < iwidth_; i++) {
                pixc = pix0 + ((i * 1.0) * du) + ((j * 1.0) * dv);
                rdir = pixc - lookfrom_;

                r = RayX<T, VECD>{lookfrom_, rdir};
                fb(i, j) = colorizer(r);
            }
        }
    }

    #pragma endregion
};

#endif