#include "core.hpp"

Sphere& Sphere::with_material(Material value) {
    m_material = value;
    return *this;
}
Sphere& Sphere::with_pos(Vec3f value) {
    m_pos = value;
    return *this;
}
Sphere& Sphere::with_radius(Float value) {
    m_radius = value;
    return *this;
}

void Sphere::format(std::ostream& out, size_t indent) const {
    write_indent(out, indent);
    out << "Sphere(pos: " << m_pos << ", rad: " << m_radius << ")";
}

Intersection Sphere::intersect(Ray ray) const { 
    // Move sphere to origin
    Vec3f x = ray.pos - m_pos;

    // Solving |x + dt| = R yields
    // t^2 + 2pt + q = 0
    // where p = x.d, q = |x|^2 - R^2

    Float p = x.dot(ray.dir);
    Float q = x.norm_squared() - m_radius * m_radius;
    Float D = p * p - q;

    if (q >= 0.0) {
        // Outside sphere
        // both solutions have same sign
        Float t = -p - std::sqrt(D);

        if (t <= 0.0) return { .hit = false };

        Vec3f y = x + t * ray.dir;
        return {
            .local = {
                .pos    = y + m_pos,
                .normal = y.unit(),
                .material = &m_material 
            },
            .dist   = t,
            .hit    = true,
        };
    }
    else {
        // Inside sphere
        // solution guaranteed to be positive
        Float t = -p + std::sqrt(D);
        Vec3f y = x + t * ray.dir;   

        return {
            .local = {
                .pos    = y + m_pos,
                .normal = -y.unit(),
                .material = &m_material
            },
            .dist   = t,
            .hit    = true, 
        };
    }
};