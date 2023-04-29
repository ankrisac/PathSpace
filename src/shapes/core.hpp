#pragma once
#include "../core.hpp"

struct Plane : public Shape {
    Material m_material; 
    Vec3f m_pos;
    Unit<Vec3f> m_dir;

    Plane& with_material(Material material);
    Plane& with_pos(Vec3f pos);
    Plane& with_dir(Unit<Vec3f> dir);
    std::shared_ptr<Shape> build() {
        auto out = std::make_shared<Plane>();
        *out = *this;
        return out;
    }

    void format(std::ostream& out, size_t indent) const override;

    Intersection intersect(Ray ray) const override;
};

struct Disc : public Shape {
    Material m_material; 
    Vec3f m_pos;
    Float m_radius;

    Unit<Vec3f> m_dir;

    Disc& with_material(Material material);
    Disc& with_pos(Vec3f pos);
    Disc& with_dir(Unit<Vec3f> dir);
    Disc& with_radius(Float radius);

    std::shared_ptr<Shape> build() {
        auto out = std::make_shared<Disc>();
        *out = *this;
        return out;
    }

    void format(std::ostream& out, size_t indent) const override;

    Intersection intersect(Ray ray) const override;
};

struct Diamond : public Shape {
    Material m_material; 

    Vec3f m_pos;
    Vec3f m_a;
    Vec3f m_b;
    Unit<Vec3f> m_dir;

    Diamond& with_material(Material material);
    Diamond& with_pos(Vec3f value);
    Diamond& with_a(Vec3f a);
    Diamond& with_b(Vec3f b);
    std::shared_ptr<Shape> build() {
        auto out = std::make_shared<Diamond>();
        *out = *this;
        out->m_dir = m_a.cross(m_b).unit();
        return out;
    }

    void format(std::ostream& out, size_t indent) const override;
    Intersection intersect(Ray ray) const override;
};


struct Sphere : public Shape {
    Material m_material; 
    Vec3f m_pos;
    Float  m_radius;

    Sphere& with_material(Material material);
    Sphere& with_pos(Vec3f value);
    Sphere& with_radius(Float value);
    std::shared_ptr<Shape> build() {
        auto out = std::make_shared<Sphere>();
        *out = *this;
        return out;
    }

    void format(std::ostream& out, size_t indent) const override;
    Intersection intersect(Ray ray) const override;
};

struct ShapeList : public Shape {
    std::vector<std::shared_ptr<Shape>> data;

    void format(std::ostream& out, size_t indent) const override;
    void add(std::shared_ptr<Shape> shape) {
        data.push_back(shape);
    }

    Intersection intersect(Ray ray) const override;
};