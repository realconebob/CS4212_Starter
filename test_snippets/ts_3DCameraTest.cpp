#include "Framebuffer.h"
#include "handleGraphicsArgs.h"

#include "PerspectiveCamera.h"
#include "PNGRenderer.h"
#include "VecX.h"
#include "Sphere.h"
#include "World.h"
#include "Hittable.h"

#include <cstddef>

using Color3D = VecX<double, 3>;
using PC3D = PerspectiveCamera3D<double>;
using FB3D = Framebuffer<double, 3>;
using Sphere3DD = Sphere3D<double>;
using World3DD = World3D<double>;
using Hittable3D = HittableAny<double, 3>;

int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    auto camera = PC3D{};
    camera.iwidth = 2000;

    auto fb = FB3D{(std::size_t)camera.get_iwidth(), (std::size_t)camera.get_iheight()};
    fb.clear();

    auto world = World3DD{
        Hittable3D{Sphere3DD(Vec3D(-1, 0, -2), 0.75)},
        Hittable3D{Sphere3DD(Vec3D(1, 0, -2), 0.5)}
    };

    camera.rendertobuffer(fb, [&world](const Ray3D& r){
        if(world.intersect(r)) return Vec3D(1, 0, 0);

        auto udir = unit(r.dir());
        auto a = 0.5 * (udir + Vec3D(1.0, 1.0, 1.0));
        return Vec3D(1.0 - a[0], 1.0 - a[1], 1.0);
    });

    // Not sure why the spheres are rendering as ovals. That's a problem for later me to solve

    PNGRenderer<double>(fb, "camera3d-white.png").render();
    exit(0);
}