#include "cwrender/BaseTypes/Interval.hpp"
#include "cwrender/CWRender.hpp"
#include "handleGraphicsArgs.h"
#include "SceneLoader.hpp"
#include "SceneParser_JSON.hpp"

#include <cstddef>
#include <memory>

using namespace cwrender;

using Color3D = VecX<double, 3>;
using PC3D = PerspectiveCamera3<double>;
using FB3D = Framebuffer<double, 3>;
using Sphere3DD = Sphere3D<double>;
using Scene3D = Scene<double, 3>;
using Hittable3D = HittableAny<double, 3>;
using HitRecord3D = HitRecord<double, 3>;
using Triangle3D = Triangle3<double>;
using Plane3D = Plane3<double>;
using Cube3D = Cube3<double>;

int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    Scene3D world;
    auto loader = std::make_shared<SceneLoader>(world);
    SceneParser_JSON(loader).parseFileData("../../sceneData/scenes_A/cwalstrom_fa26.json");
    loader->finalize();

    auto camera = loader->makeCamera(16.0 / 9, 1000);   // aspect, image width
    camera.samplegrid = 8;

    auto fb = FB3D{(std::size_t)camera.get_iwidth(), (std::size_t)camera.get_iheight()};
    fb.clear();

    HitRecord3D rec;
    camera.rendertobuffer(fb, [&rec, &world, &camera](const Ray3D& r) {
        return camera.raycolor(r, world, world, rec, Interval<double>::camera(), 50);   // rec/range as in your main
    });

    PNGRenderer<double>(fb, "out.png").render(true);
}