#include "handleGraphicsArgs.h"

#include "Camera3D.h"
#include "PNGRenderer.h"

#include <cstddef>

using Color3D = VecX<double, 3>;

int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    auto aspect_ratio = 16.0 / 9.0;
    int 
        width = 400,
        height {int(width / aspect_ratio)};
    height = (height < 1) ? 1 : height;

    auto vheight = 2.0;
    auto vwidth = vheight * (double(width)/height);

    auto fb = Framebuffer<double, 3>{(std::size_t)width, (std::size_t)height};
    fb.clear();

    auto camera = Camera3D<double>(width, height, vwidth, vheight, 1);
    camera.rendertobuffer(fb, [](const RayX<double, 3>& r){
        Vec3D udir = unit(r.dir());
        auto a = 0.5 * (udir[1] + 1.0);
        return (1.0-a)*Vec3D(1.0, 1.0, 1.0) + a*Vec3D(0.5, 0.7, 1.0);
    });

    PNGRenderer<double>(fb, "camera3d-white.png").render();
    exit(0);
}