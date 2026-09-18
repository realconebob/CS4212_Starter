#ifndef CS4212_GRAPHICS_CAMERA3D__78361307319100__
#define CS4212_GRAPHICS_CAMERA3D__78361307319100__

#include "framebuffer.h"
#include "helpers.h"
#include "RayX.h"
#include "vecx.h"

#include <cstddef>

template <typename F, typename T, std::size_t X>
concept RayColorizer = std::is_invocable_r_v<VecX<T, X>, F, const RayX<T, X>&>;

template<Floating T>
class Camera3D {
    private:
    static const int VECD = 3;

    protected:
    #pragma region Variables
    VecX<T, VECD> origin_;
    
    double 
        cwidth_,
        cheight_,
        focallen_;

    double 
        iwidth_,
        iheight_;

    VecX<T, VECD> u_, v_;
    #pragma endregion

    #pragma region Protected Helper Functions
    const VecX<T, VECD> deltaU() const {return u_ / iwidth_;}
    const VecX<T, VECD> deltaV() const {return v_ / iheight_;}
    const VecX<T, VECD> upperleft() const {return origin_ - VecX<T, VECD>{0, 0, focallen_} - (u_/2.0) - (v_/2.0);}
    const VecX<T, VECD> pix00loc() const {return upperleft() + 0.5 * (deltaU() + deltaV());}

    #pragma endregion

    public:
    #pragma region Constructors
    Camera3D() = delete;
    Camera3D(double iwidth, double iheight, double vwidth, double vheight, double focallen): // If this were any longer I'd consider a builder
        cwidth_{vwidth}, cheight_{vheight}, focallen_{focallen},            // Viewport variables
        u_{VecX<T, VECD>{vwidth, 0, 0}}, v_{VecX<T, VECD>{0, -vheight, 0}}, // Helper U and V vectors
        iwidth_{iwidth}, iheight_{iheight}                                  // Image variables
        {}
    
    #pragma endregion

    #pragma region Misc

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
                rdir = pixc - origin_;

                r = RayX<T, VECD>{origin_, rdir};
                fb(i, j) = colorizer(r);
            }
        }
    }

    #pragma endregion
};

#endif