#pragma once
#include "prelude.hpp"
#include "graphics/core.hpp"
#include "pcg_random.hpp"

#include <exception>
#include <random>

struct Random {
    pcg32 rng;
    std::uniform_real_distribution<Float> dist;
    std::normal_distribution<Float> dist2;

    Random(): dist(0.0, 1.0), dist2(0.0, 1.0) {
        std::random_device device;
        rng.seed(device());
    }

    Float unit() {
        return dist(rng);
    }

    Vec2f unit2D(){
        return { 
            unit(),
            unit()
        };
    }
    Float range(Float a, Float b) {
        return a + unit() * (b - a);
    }

    Vec3f sample_ball() {
        return Vec3f(dist2(rng), dist2(rng), dist2(rng));
    }
    Vec3f sample_sphere() {
        return sample_ball().unit();
    }
    Vec3f sample_hemisphere(Vec3f normal) {
        return sample_sphere().facing(normal);
    }
    Vec3f sample_cos_hemisphere(Vec3f normal) {
        return (sample_sphere() + normal).unit();
    }
};

struct Ray {
    Vec3f pos;
    Unit<Vec3f> dir;
};

using Spectrum = Color;

struct Material;

struct LocalSurface {
    Vec3f pos = { 0.0, 0.0, 0.0 };    
    Unit<Vec3f> normal = { 0.0, 0.0, 0.0 };
    const Material* material = nullptr;
};

static Vec3f sample_sphere(Vec2f u) {
    Float z   = 1 - 2 * u.x;
    Float r   = std::sqrt(std::max(Float(0), 1.0 - z * z));
    Float phi = 2 * M_PI * u.y;

    return { r * std::cos(phi), r * std::sin(phi), z };
}

struct Material {
    Spectrum diffuse  = Spectrum(1.0);
    Spectrum specular = Spectrum(0.0);
    Spectrum emission = Spectrum(0.0);

    Float prob_specular = 0.0;
    std::pair<Ray, Spectrum> sample_f(Random* rng, const LocalSurface* surface) const {
        Ray new_ray;
        new_ray.pos = surface->pos + 0.0001 * surface->normal;
        new_ray.dir = rng->sample_cos_hemisphere(surface->normal);

        Float cos_theta = new_ray.dir.dot(surface->normal);
        Spectrum output = diffuse;

        return { new_ray, output };
    };
};


struct Intersection {
    LocalSurface local = {};
    Float dist = INFINITY;
    bool hit = false;
};

struct Shape {
    virtual ~Shape() {}
    virtual void format(std::ostream& out, size_t indent) const {
        out << "Shape";
    }
    virtual void draw(Canvas& cnv) const {};
    virtual Intersection intersect(Ray ray) const { 
        throw std::logic_error("Shape: No intersection implemented!");
    };
};

struct Scene : public Shape {
    const Shape* shape; 
    std::vector<const Shape*> data;
};

inline std::ostream& operator<<(std::ostream& out, const Shape* shape) {
    shape->format(out, 0);
    return out;
}


struct Integrator {
    Random rng;
    const Shape* scene;
    std::vector<const Shape*> lights;

    Integrator() {}

    Spectrum pathtrace(Ray ray) {
        Spectrum lum = Colors::BLACK;
        Spectrum BSDF_prod = Colors::WHITE;

        for(size_t i = 0; i < 10; i++) {
            Intersection hit = scene->intersect(ray);

            if(!hit.hit) break;

            const LocalSurface* surface = &hit.local;
            auto [new_ray, BSDF] = surface->material->sample_f(&rng, surface);

            lum += BSDF_prod * surface->material->emission;
            BSDF_prod *= BSDF;

            ray = new_ray;
        }

        return lum;
    }
};
