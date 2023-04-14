#include <glm/glm.hpp>
#include <fmt/core.h>

#include "prelude.hpp"
#include "canvas.hpp"

#include <random>

int main(int argc, char **argv) {
    Size size = { 1024, 1024 };
    
    Window window { size };

    Frame frame {};
    Grid2D<Color> image { size, {1.0, 1.0, 1.0, 1.0 }};

    Canvas cnv;

    f32 t = 0.0;

    while(!window.should_close()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1.0);

        //frame.draw(image);

        cnv.line({ -0.5, -0.5 }, { 0.5, 0.5 });
        cnv.circle({ 0.5, 0.0 }, 0.2);

        cnv.draw();

        t += 0.01;

        window.update();
    }

    return 0;
}
