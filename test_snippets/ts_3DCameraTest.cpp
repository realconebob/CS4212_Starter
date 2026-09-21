#include "handleGraphicsArgs.h"

#include "Camera3D.h"
#include "PNGRenderer.h"

#include <cstddef>

using Color3D = VecX<double, 3>;

int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    auto camera = Camera3D<double>{};
    auto fb = Framebuffer<double, 3>{(std::size_t)camera.get_imagewidth(), (std::size_t)camera.get_imageheight()};
    fb.clear();

    camera.rendertobuffer(fb, [](const RayX<double, 3>& r){
        Vec3D udir = unit(r.dir());
        auto a = 0.5 * (udir[1] + 1.0);
        return (1.0-a)*Vec3D(1.0, 1.0, 1.0) + a*Vec3D(0.5, 0.7, 1.0);
    });

    PNGRenderer<double>(fb, "camera3d-white.png").render();
    exit(0);
}