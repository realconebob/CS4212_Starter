#include "Framebuffer.h"
#include "handleGraphicsArgs.h"

#include "PerspectiveCamera.h"
#include "PNGRenderer.h"
#include "VecX.h"
#include "Sphere.h"
#include "World.h"
#include "Hittable.h"
#include "Triangle.h"

#include <cstddef>

using Color3D = VecX<double, 3>;
using PC3D = PerspectiveCamera3D<double>;
using FB3D = Framebuffer<double, 3>;
using Sphere3DD = Sphere3D<double>;
using World3DD = World3D<double>;
using Hittable3D = HittableAny<double, 3>;
using HitRecord3D = HitRecord<double, 3>;
using Triangle3D = Triangle3<double>;

int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    auto camera = PC3D{};
    camera.iwidth = 200;
    camera.aspectratio = 1;
    camera.vfov = 100;

    auto fb = FB3D{(std::size_t)camera.get_iwidth(), (std::size_t)camera.get_iheight()};
    fb.clear();

    World3DD world {
        Hittable3D{Sphere3DD(Vec3D(-1, 0, -4), 0.75)},
        Hittable3D{Triangle3D(Vec3D(-0.5, 0, -3), Vec3D(1, 1, -3), Vec3D(1, -1, -3))},
        Hittable3D{Sphere3DD(Vec3D(1, 0, -2), 0.5)}
    };
    auto rec = HitRecord3D{};

    camera.rendertobuffer(fb, [&world, &rec](const Ray3D& r){
        if(world.intersect(r, rec)) return 0.5 * Vec3D{rec.normal()[0] + 1, rec.normal()[1] + 1, rec.normal()[2] + 1};

        auto udir = unit(r.dir());
        auto a = 0.5 * (udir + Vec3D(1.0, 1.0, 1.0));
        return Vec3D(a[0], a[1], a[2]);
    });

    // Not sure why the spheres are rendering as ovals. That's a problem for later me to solve

    PNGRenderer<double>(fb, "camera3d-white.png").render();
    exit(0);
}