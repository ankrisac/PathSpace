#include "core.hpp"

Plane& Plane::with_material(Material value) {
    m_material = value;
    return *this;
}
Plane& Plane::with_pos(Vec3f value) {
    m_pos = value;
    return *this;
}
Plane& Plane::with_dir(Unit<Vec3f> value) {
    m_dir = value;
    return *this;
}

void Plane::format(std::ostream& out, size_t indent) const {
    write_indent(out, indent);
    out << "Plane(pos: " << m_pos << ", dir: " << m_dir << ")";
}

Intersection Plane::intersect(Ray ray) const {    
    // Component of ray along plane normal
    Float ray_n = m_dir.dot(ray.dir);

    // Parallel to plane
    if(ray_n == 0.0) return { .hit = false };

    Float t = m_dir.dot(m_pos - ray.pos) / ray_n;
    if(t <= 0.0) return { .hit = false };

    return Intersection {
        .local = {
            .pos      = ray.pos + ray.dir * t,
            .normal   = (ray_n > 0.0) ? -m_dir : m_dir,
            .material = &m_material,
        },
        .dist = t,
        .hit = true
    };
}
