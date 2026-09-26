#include "handleGraphicsArgs.h"

#include "PerspectiveCamera3D.h"
#include "PNGRenderer.h"
#include "vecx.h"
#include "Sphere.h"
#include "World.h"
#include "Hittable.h"

#include <cstddef>

using Color3D = VecX<double, 3>;

int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    auto camera = PerspectiveCamera3D<double>{};
    camera.iwidth = 2000;

    auto fb = Framebuffer<double, 3>{(std::size_t)camera.get_iwidth(), (std::size_t)camera.get_iheight()};
    fb.clear();

    auto sphere1 = Sphere3D<double>(Vec3D(-2, 0, -2), 1);
    auto sphere2 = Sphere3D<double>(Vec3D(2, 0, -2), 1);
    auto world = World3D<double>{HittableAny<double, 3>{sphere1}, HittableAny<double,3>{sphere2}};

    camera.rendertobuffer(fb, [&world](const RayX<double, 3>& r){
        if(world.intersect(r)) return Vec3D(1, 0, 0);

        auto udir = unit(r.dir());
        auto a = 0.5 * (udir + Vec3D(1.0, 1.0, 1.0));
        return Vec3D(a[0], a[1], 1.0);
    });

    // Not sure why the spheres are rendering as ovals. That's a problem for later me to solve

    PNGRenderer<double>(fb, "camera3d-white.png").render();
    exit(0);
}