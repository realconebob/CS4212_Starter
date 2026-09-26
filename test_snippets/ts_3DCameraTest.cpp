#include "handleGraphicsArgs.h"

#include "Camera3D.h"
#include "PNGRenderer.h"
#include "vecx.h"
#include "Sphere.h"

#include <cstddef>

using Color3D = VecX<double, 3>;

int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    auto cb = Camera3D<double>::Builder{}
        .set_lookfrom(Vec3D{0, 0, 0})
        .set_lookat(Vec3D{0, 0, -1})
        .set_aspect(16.0/9)
        .set_vfov(90);

    auto camera = Camera3D<double>{cb};
    auto fb = Framebuffer<double, 3>{(std::size_t)camera.get_imagewidth(), (std::size_t)camera.get_imageheight()};
    fb.clear();

    auto sphere = Sphere3D<double>(Vec3D(0, 0, -2), 1);

    camera.rendertobuffer(fb, [&sphere](const RayX<double, 3>& r){
        if(sphere._intersect(r)) return Vec3D(1, 0, 0);

        auto udir = unit(r.dir());
        auto a = 0.5 * (udir + Vec3D(1.0, 1.0, 1.0));
        return Vec3D(a[0], a[1], 1.0);
    });

    PNGRenderer<double>(fb, "camera3d-white.png").render();
    exit(0);
}