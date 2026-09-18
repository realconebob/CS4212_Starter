#include "handleGraphicsArgs.h"

#include "Camera3D.h"
#include "PNGRenderer.h"

#include <cstddef>

using Color3D = VecX<double, 3>;

int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    auto fb = Framebuffer<double, 3>{(std::size_t)args.width, (std::size_t)args.height};
    fb.clear();

    auto camera = Camera3D<double>(args.width, args.height, args.width, args.height, 1);
    camera.rendertobuffer(fb, [](const RayX<double, 3>& ray){
        return Vec3D{1, 0, 0};
    });

    PNGRenderer<double>(fb, "camera3d-white.png").render();
    exit(0);
}