
#include "vecx.h"
#include "framebuffer.h"
#include "PNGRenderer.h"

#include "handleGraphicsArgs.h"

#include <cstddef>

using Color3D = VecX<double, 3>;

int main(int argc, char *argv[]) {
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    auto fb = Framebuffer<double, 3>{(std::size_t)args.width, (std::size_t)args.height};
    fb.clear();
    
    // Render a black image
    PNGRenderer<double>(fb, "black.png").render();

    // Render a fade from blue to red
    fb.clear_to(Color3D{0, 0, 1}, Color3D{1, 0, 0});
    PNGRenderer<double>(fb, "blue-red-fade.png").render();

    // Render a rainbow fade
    fb.clear_to(
        Color3D{1, 0, 0},   // Red
        Color3D{1, .5, 0},  // Orange
        Color3D{1, 1, 0},   // Yellow
        Color3D{0, 0, 0},   // Green
        Color3D{0, 0, 1},   // Blue
        Color3D{.3, 0, .51},// Indigo 
        Color3D{.58, 0, .83}// Violet
    );
    PNGRenderer<double>(fb, "rainbow-fade.png").render();

    exit(0);
}