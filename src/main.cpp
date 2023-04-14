#include <glm/glm.hpp>
#include <fmt/core.h>

#include "prelude.hpp"
#include "canvas.hpp"

int main(int argc, char **argv) {
    Window window { {512, 512} };

    Frame frame {};
    Grid2D<Color> image {{512, 512}, {0.0, 1.0, 1.0, 1.0}};

    while(!window.should_close()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0, 0.0, 0.0, 1.0);

        frame.draw(image);
        window.update();
    }

    return 0;
}
