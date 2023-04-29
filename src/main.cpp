#include "prelude.hpp"
#include "graphics/core.hpp"

#include "core.hpp"
#include "shapes/core.hpp"



#include <fstream>
#include <random>
#include <thread>




int main(int argc, char **argv) {

    ShapeList scene;

    scene.add(
        Sphere {}
            .with_pos({ -0.3, -0.6, 0.50 })
            .with_radius(0.4)
            .with_material({ .diffuse = 0.7 * Colors::WHITE })
            .build()
    );

    std::pair<Vec3f, Material> planes[] = {
        { {-1.0, 0.0, 0.0 }, Material { .diffuse = 0.95 * Colors::RED   } },
        { {+1.0, 0.0, 0.0 }, Material { .diffuse = 0.95 * Colors::GREEN } },
        { { 0.0,-1.0, 0.0 }, Material { .diffuse = 0.95 * Colors::WHITE } },
        { { 0.0,+1.0, 0.0 }, Material { .diffuse = 0.95 * Colors::WHITE } },
        { 
            { 0.0, 0.0,-1.0 }, 
            Material { 
                .diffuse = 0.7 * Colors::WHITE,
                .specular = 0.8 * Colors::WHITE,
                .prob_specular = 0.7  
            } 
        },
        { { 0.0, 0.0,+1.0 }, Material { .diffuse = 0.8 * Colors::WHITE } },        
    };
    for (auto [pos, material] : planes) {
        scene.add(
            Plane{}
                .with_pos(pos)
                .with_dir(-pos)
                .with_material(material)
                .build()
        );
    }

    Diamond bulb = Diamond{}
        .with_pos({ 1.0, 0.99, 1.0 })
        .with_a({ 0.25, 0.0, 0.0 })
        .with_b({ 0.0, 0.0, 0.25 })
        .with_material({
            .emission = Color { 10.0, 10.0, 10.0 }
        });

    scene.add(bulb.build());
    std::vector<const Shape*> lights = {
        &bulb
    };

    std::cout << &scene << std::endl;    

    Window window { { 1024, 1024 } };
    Frame frame { { 512, 512 } };

    std::vector<std::thread> workers;
    for(size_t i = 0; i < 12; i++) {
        workers.emplace_back([&frame, &scene, &lights]{
            Integrator solver;
            solver.scene = &scene;
            solver.lights = lights;

            Random rng;

            Vec2f  size = frame.get_size().cast<Float>();
            Float aspect = size.x / size.y;

            while(true) {
                for(size_t j = 0; j < 256; j++) {
                    for(size_t i = 0; i < 256; i++) {
                        Vec2f uv = (Vec2f(i, j) + rng.unit2D()) * (1.0 / 256.0);

                        Vec2f screen = { 
                            (2.0f * uv.x - 1.0f) * aspect,
                            (2.0f * uv.y - 1.0f)
                        };
                        Ray ray = {
                            .pos = Vec3f(0.0, 0.0, -0.95),
                            .dir = screen.with_z(1.0).unit()
                        };

                        Color sample = solver.pathtrace(ray);
                        frame.add_bilinear(uv, sample);
                    }
                }
            }
        });
    }

    while(!window.should_close()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1.0);

        frame.render();        
        window.update();
    }

    for(auto& worker : workers) {
        worker.join();
    }

    return 0;
}
