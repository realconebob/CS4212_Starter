#include "Framebuffer.h"
#include "Interval.h"
#include "NormalShader.h"
#include "Plane.h"
#include "handleGraphicsArgs.h"

#include "PerspectiveCamera.h"
#include "PNGRenderer.h"
#include "VecX.h"
#include "Sphere.h"
#include "World.h"
#include "Hittable.h"
#include "Triangle.h"
#include "Cube.h"
#include "Material.h"
#include "LambertianShader.h"

#include <cstddef>
#include <memory>

using Color3D = VecX<double, 3>;
using PC3D = PerspectiveCamera3<double>;
using FB3D = Framebuffer<double, 3>;
using Sphere3DD = Sphere3D<double>;
using World3DD = World3D<double>;
using Hittable3D = HittableAny<double, 3>;
using HitRecord3D = HitRecord<double, 3>;
using Triangle3D = Triangle3<double>;
using Plane3D = Plane3<double>;
using Cube3D = Cube3<double>;

int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    auto camera = PC3D{};
    camera.iwidth = 750;
    camera.aspectratio = 1;
    camera.vfov = 75;

    auto fb = FB3D{(std::size_t)camera.get_iwidth(), (std::size_t)camera.get_iheight()};
    fb.clear();

    auto mat = std::make_shared<MaterialAny<double, 3>>(Lambertian<double, 3>());
    World3DD world {
        Hittable3D{Sphere3DD(Vec3D(-1, 0, -2), 0.75, mat)},
        // Hittable3D{Triangle3D(Vec3D(-0.5, 0, -3), Vec3D(1, 1, -3), Vec3D(1, -1, -3))},
        Hittable3D{Sphere3DD(Vec3D(1, 0, -1), 0.5, mat)},
        // Hittable3D{Plane3D{{-1, 1, -3}, {1, -1, -2}}},
        // Hittable3D{Cube3D{{-0.5, 0.5, -2}, {0.5, -0.5, -1}}}
    };
    auto rec = HitRecord3D{};
    auto range = Interval<double>::camera();

    camera.rendertobuffer(fb, [&camera, &world, &rec, &range](const Ray3D& r){
        return camera.raycolor(r, world, rec, range, 50);
    });

    // Not sure why the spheres are rendering as ovals. That's a problem for later me to solve

    PNGRenderer<double>(fb, "camera3d-white.png").render();
    exit(0);
}