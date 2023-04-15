#include "prelude.hpp"
#include "gfx.hpp"
#include "canvas.hpp"
#include "frame.hpp"

#include <fstream>
#include <random>
#include <thread>

#include "shapes/core.hpp"

void raytrace(Canvas& cnv, const Shape& scene, Ray ray) {    
    for(size_t i = 0; i < 100; i++) {
        Hit hit = scene.intersect(ray);        

        if(!hit.hit) {
            cnv.arrow(ray.pos, 2.0f * ray.dir);
            break;
        }
        cnv.line(ray.pos, hit.pos);

        //hit.draw(cnv);

        ray.dir = glm::refract(ray.dir, hit.normal, 0.5f);
        ray.pos = hit.pos + 0.001f * ray.dir;
    }
}

int main(int argc, char **argv) {
    Size size = { 1024, 1024 };
    
    Window window { size };

    Frame frame {};
    Grid2D<Color> image { size, {1.0, 1.0, 1.0, 1.0 }};

    f32 t = 0.0;
    Canvas cnv;

    Ray ray;

    Scene scene;
    scene.data.push_back(
        std::make_unique<Sphere>(
            Vec3(-0.5, 0.0, 0.0), 
            0.25
        )
    );
    scene.data.push_back(
        std::make_unique<Plane>(
            Vec3(0.0, -0.5, 0.0), 
            Vec3(0.0, +1.0, 0.0)
        )
    );

    while(!window.should_close()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1.0);

        Vec3 pos = Vec3(window.get_mouse_pos(), 0.0);

        if (window.mouse.pressed()) ray.pos = pos;
        ray.dir = glm::normalize(pos - ray.pos);

        cnv.clear();
        cnv.style.width = 0.01;
        scene.draw(cnv);    
        cnv.arrow(ray.pos, 0.2f * ray.dir);
        cnv.render();

        //frame.render(image);
        //cnv.save_as_svg("example.svg");

        t += 0.01;

        window.update();
    }

    return 0;
}
