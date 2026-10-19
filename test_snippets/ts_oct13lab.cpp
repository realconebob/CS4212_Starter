#include "cwrender/BaseTypes/Interval.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/Hittables/Sphere.hpp"
#include "cwrender/Hittables/Triangle.hpp"
#include "cwrender/Materials/BlinnPhongShader.hpp"
#include "cwrender/Materials/DiffuseShader.hpp"
#include "cwrender/Materials/LambertianShader.hpp"
#include "cwrender/Materials/MetalShader.hpp"
#include "cwrender/Materials/NormalShader.hpp"
#include "cwrender/Scenes/Framebuffer.hpp"
#include "cwrender/Scenes/PNGRenderer.hpp"
#include "cwrender/Scenes/PerspectiveCamera.hpp"
#include "cwrender/Scenes/PointLight.hpp"
#include "cwrender/Scenes/Scene.hpp"

using namespace cwrender;

using PC3D = PerspectiveCamera3<double>;
using FB3D = Framebuffer<double, 3>;
using Scene3D = Scene<double, 3>;
using PNGRendererD = PNGRenderer<double>;

using Record3D = HitRecord<double, 3>;
using IntervalD = Interval<double>;
using PointLight3D = PointLight<double, 3>;
using Triangle3D = Triangle3<double>;

int main(void) {
    auto camera = PC3D{};
    camera.iwidth = 500;
    camera.aspectratio = 1;
    camera.samplegrid = 10;

    camera.lookfrom = {0, 3, 3};
    camera.lookat = {0, -0.4, -1};
    camera.set_focal(0.4, 0.5);
    // Set plane widths later, once they're done

    auto
        lamgrey = LambertianShader<double, 3>{{.6, .6, .6}}.sharedptr(),
        lamyell = LambertianShader<double, 3>{{.9, .8, .1}}.sharedptr(),
        mirror = MetalShader<double, 3>{{1, 1, 1}}.sharedptr(),
        normal = NormalMapShader<double, 3>().sharedptr(),
        lamgreen = LambertianShader<double, 3>{{.2, .8, .2}}.sharedptr(),
        blinred = BlinnPhongShader<double, 3>{{}, 64, {.8, .1, .1}}.sharedptr(),
        blinblue = BlinnPhongShader<double, 3>{{}, 160, {.1, .2, .8}}.sharedptr(),
        difteal = DiffuseShader<double, 3>({.2, .7, .7}).sharedptr(),
        blinorangle = BlinnPhongShader<double, 3>{{}, 32, {.95, .5, .1}}.sharedptr(),
        lampurple = LambertianShader<double, 3>{{.6, .2, .8}}.sharedptr();

    auto fb = FB3D(camera.get_iwidth(), camera.get_iheight());
    auto rec = Record3D();
    auto range = IntervalD::camera();
    auto world = Scene3D{
        {
            // Triangles
            /* Ground1 */   Triangle3D({-50, 0, 50}, {50, 0, 50}, {50, 0, -50}, lamgrey),
            /* Ground2 */   Triangle3D({-50, 0, 50}, {50, 0, -50}, {-50, 0, -50}, lamgrey),
            /* Wall1 */     Triangle3D({-4, 0, -6}, {4, 0, -6}, {4, 4, -6}, mirror),
            /* Wall2 */     Triangle3D({-4, 0, -6}, {4, 4, -6}, {-4, 4, -6}, mirror),
            /* Floating */  Triangle3D({0.9, 1.4, -0.6}, {1.9, 1.4, -0.6}, {1.4, 2.2, -1.1}, lamyell),

            // Spheres
            /* Green */     Sphere3D<double>({-2, 0.8, -3}, 0.8, lamgreen).any(),
            /* Red */       Sphere3D<double>({-1.1, 0.6, -1.2}, 0.6, blinred).any(),
            /* Normal */    Sphere3D<double>({0, 0.9, -3.5}, 0.9, normal).any(),
            /* Blue */      Sphere3D<double>({0.3, 0.45, -1.6}, 0.45, blinblue).any(),
            /* Mirror1 */   Sphere3D<double>({2, 1, -3}, 1, mirror).any(),
            /* Mirror2 */   Sphere3D<double>({1.5, 0.5, 0.3}, 0.5, mirror).any(),
            /* teal */      Sphere3D<double>({-1.3, 0.45, 0.4}, 0.45, difteal).any(),

            // Behind camera
            /* Orange */    Sphere3D<double>({0.3, 1, 5}, 1, blinorangle).any(),
            /* Purple */    Sphere3D<double>({2.4, 0.8, 4.8}, 0.8, lampurple).any(),
        },
        {
            PointLight3D{{-5, 8, 5}, {7, 6, 45}},
            PointLight3D{{6, 6, 2}, {35, 45, 7}},
        }
    };

    camera.rendertobuffer(fb, [&camera, &world, &rec, &range](const Ray3D& r){
        return camera.raycolor(r, world, world, rec, range, 5);
    });
    PNGRendererD(fb, "Oct13Lab.png").render(true);

    return 0;
}