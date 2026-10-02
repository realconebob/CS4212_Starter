#include "cwrender/BaseTypes/RayX.hpp"
#include "cwrender/Materials/MetalShader.hpp"
#include "cwrender/Scenes/Framebuffer.hpp"
#include "cwrender/Scenes/PerspectiveCamera.hpp"
#include "cwrender/Scenes/PNGRenderer.hpp"

#include "cwrender/BaseTypes/VecX.hpp"
#include "cwrender/BaseTypes/Interval.hpp"

#include "cwrender/Hittables/Sphere.hpp"
#include "cwrender/Hittables/World.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/Hittables/Triangle.hpp"
#include "cwrender/Hittables/Cube.hpp"
#include "cwrender/Hittables/Plane.hpp"


#include "cwrender/Materials/Material.hpp"
#include "cwrender/Materials/LambertianShader.hpp"
#include "cwrender/Materials/NormalShader.hpp"

#include "handleGraphicsArgs.h"

#include <cstddef>
#include <functional>
#include <memory>

using namespace cwrender;

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
    camera.iwidth = 5000;
    camera.aspectratio = 1;
    camera.vfov = 30;

    auto fb = FB3D{(std::size_t)camera.get_iwidth(), (std::size_t)camera.get_iheight()};
    fb.clear();

    auto normalmat = NormalMapShader<double, 3>().sharedptr();
    auto diffusemat = Lambertian<double, 3>(0.5 * Vec3D::Ones()).sharedptr();
    auto metalmat = MetalShader<double, 3>().sharedptr();
    World3DD world {
        Sphere3DD(Vec3D(-2, 0, -20), 1, normalmat).any(),
        Sphere3DD(Vec3D(0, 0, -15), 0.5, metalmat).any(),
        Sphere3DD(Vec3D(1, -0.75, -10), 0.75, diffusemat).any(),
        Plane3D{{-100, 0, -60}, {100, -2, 0}, diffusemat}.any(),
    };
    auto rec = HitRecord3D{};
    auto range = Interval<double>::camera();

    camera.rendertobuffer(fb, [&camera, &world, &rec, &range](const Ray3D& r){
        return camera.raycolor(r, world, rec, range, 50);
    });

    PNGRenderer<double>(fb, "camera3d-white.png").render(true);
    exit(0);
}