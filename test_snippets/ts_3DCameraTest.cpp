#include "handleGraphicsArgs.h"

#include "Camera3D.h"
#include "PNGRenderer.h"

#include <cstddef>

using Color3D = VecX<double, 3>;

int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    auto cb = Camera3D<double>::Builder{}
        .set_lookfrom(VecX<double, 3>{1, 1, 2})
        .set_lookat(VecX<double, 3>{0, 0, -1})
        .set_aspect(1.0)
        .set_vfov(120);

    auto camera = Camera3D<double>{cb};
    auto fb = Framebuffer<double, 3>{(std::size_t)camera.get_imagewidth(), (std::size_t)camera.get_imageheight()};
    fb.clear();

    camera.rendertobuffer(fb, [](const RayX<double, 3>& r){
        auto udir = unit(r.dir());
        auto a = 0.5 * (udir + Vec3D(1.0, 1.0, 1.0));
        return Vec3D(a[0], a[1], 1.0);
    });

    PNGRenderer<double>(fb, "camera3d-white.png").render();
    exit(0);
}