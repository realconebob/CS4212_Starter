#include "cwrender/BaseTypes/RayX.hpp"

#include "cwrender/Materials/BlinnPhongShader.hpp"
#include "cwrender/Renderable.hpp"
#include "cwrender/Scenes/Framebuffer.hpp"
#include "cwrender/Scenes/PerspectiveCamera.hpp"
#include "cwrender/Scenes/PNGRenderer.hpp"

#include "cwrender/BaseTypes/VecX.hpp"
#include "cwrender/BaseTypes/Interval.hpp"

#include "cwrender/Hittables/Sphere.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/Hittables/Triangle.hpp"
#include "cwrender/Hittables/Cube.hpp"
#include "cwrender/Hittables/Plane.hpp"

#include "cwrender/Materials/DiffuseShader.hpp"
#include "cwrender/Materials/NormalShader.hpp"
#include "cwrender/Materials/MetalShader.hpp"
#include "cwrender/Materials/LambertianShader.hpp"

#include "cwrender/Scenes/PointLight.hpp"
#include "cwrender/Scenes/Scene.hpp"
#include "handleGraphicsArgs.h"

#include <cstddef>
#include <memory>

using namespace cwrender;

using Color3D = VecX<double, 3>;
using PC3D = PerspectiveCamera3<double>;
using FB3D = Framebuffer<double, 3>;
using Sphere3D = Sphere3<double>;
using Scene3D = Scene<double, 3>;
using Hittable3D = Hittable<double, 3>;
using HitRecord3D = HitRecord<double, 3>;
using Triangle3D = Triangle3<double>;
using Plane3D = Plane3<double>;
using Cube3D = Cube3<double>;
using Renderable3D = Renderable<double, 3>;

int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    auto camera = PC3D{{-1, 1, -30}, {0, 0, -10}};
    camera.iwidth = 1000;
    camera.aspectratio = 16.0/9;
    camera.set_fov(20);
    camera.samplegrid = 8;

    auto fb = FB3D{(std::size_t)camera.get_iwidth(), (std::size_t)camera.get_iheight()};
    fb.clear();

    auto normalmat      = NormalMapShader<double, 3>();
    auto bluemat          = DiffuseShader<double, 3>({0, 0, 1});
    auto whitemat         = DiffuseShader<double, 3>(0.5 * Vec3D::Ones());
    auto metalmat           = MetalShader<double, 3>({0.8, 0.6, 0.2}, 0.05);
    auto lambertian    = LambertianShader<double, 3>({1, 1, 0});
    auto bphong        = BlinnPhongShader<double, 3>({10, 10, 10}, 70);

    Scene3D world {
        Renderable3D{Sphere3D(Vec3D(-2, 0, -20), 1), normalmat},
        Renderable3D{Sphere3D(Vec3D(0, 0, -15), 0.5), metalmat},
        Renderable3D{Sphere3D(Vec3D(1, -0.75, -10), 0.75), bluemat},
        Renderable3D{Sphere3D(Vec3D(1, 1, -10), 0.3), lambertian},
        Renderable3D{Sphere3D(Vec3D(-1, 1, -10), 0.3), bphong},
        Renderable3D{Plane3D{{-100, -1, -60}, {100, -1, 0}}, whitemat},
        PointLight<double, 3>{{0, 2, -11}, {100, 0, 100}},
        PointLight<double, 3>{{0, 0, -11}, {100, 100, 100}},
    };
    auto range = Interval<double>::camera();

    camera.rendertobuffer(fb, [&camera, &world, &range](const RayX<double, 3>& r){
        return camera.raycolor(world, r, range, 5);
    });

    PNGRenderer<double>(fb, "camera3d-white.png").render(true);
    exit(0);
}