#include "../prelude.hpp"
#include "../canvas.hpp"



struct Ray {
    Vec3 pos = { 0.0, 0.0, 0.0 };
    Vec3 dir = { 0.0, 0.0, 0.0 };
};

struct Hit {
    Vec3 pos = { 0.0, 0.0, 0.0 };
    Vec3 normal = { 0.0, 0.0, 0.0 };
    f32 dist = INFINITY;
    bool hit = false;

    void draw(Canvas& cnv) {
        cnv.arrow(pos, 5 * cnv.style.width * normal);
    }
};



struct Shape {
    virtual void draw(Canvas& cnv) const {};
    virtual Hit intersect(Ray ray) const { return {}; };
};

struct Plane : public Shape {
    Vec3 m_pos;
    Vec3 m_normal;

    Plane(Vec3 pos, Vec3 normal);

    void draw(Canvas& cnv) const override;
    Hit intersect(Ray ray) const override;
};

struct Sphere : public Shape {
    Vec3 m_pos;
    f32  m_radius;

    Sphere(Vec3 pos, f32 radius);
    
    void draw(Canvas& cnv) const override;
    Hit intersect(Ray ray) const override;
};



struct Scene : public Shape {
    std::vector<std::unique_ptr<Shape>> data;
    
    void draw(Canvas& cnv) const {
        for (auto& shape : this->data) {
            shape->draw(cnv);
        } 
    }
    Hit intersect(Ray ray) const {
        Hit nearest;

        for (auto& shape : this->data) {
            Hit hit = shape->intersect(ray);

            if (nearest.dist > hit.dist) {
                nearest = hit;
            }
        }

        return nearest;
    }
};