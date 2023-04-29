#include "core.hpp"

Disc& Disc::with_material(Material value) {
    m_material = value;
    return *this;
}
Disc& Disc::with_pos(Vec3f value) {
    m_pos = value;
    return *this;
}
Disc& Disc::with_dir(Unit<Vec3f> value) {
    m_dir = value;
    return *this;
}
Disc& Disc::with_radius(Float value) {
    m_radius = value;
    return *this;
}

void Disc::format(std::ostream& out, size_t indent) const {
    write_indent(out, indent);
    out << "Disc(pos: " << m_pos << ", dir: " << m_dir << ")";
}

Intersection Disc::intersect(Ray ray) const {    
    // Component of ray along Disc normal
    Float ray_n = m_dir.dot(ray.dir);

    // Parallel to Disc
    if(ray_n == 0.0) {
        return { .hit = false };
    }

    // First intersection
    Float t = m_dir.dot(m_pos - ray.pos) / ray_n;
    if(t <= 0.0) {
        return { .hit = false };
    }

    Vec3f pos = ray.pos + ray.dir * t;

    // Outside circle
    if((pos - m_pos).norm_squared() > m_radius * m_radius) {
        return { .hit = false };
    }

    return Intersection {
        .local = {
            .pos      = pos,
            .normal   = (ray_n > 0.0) ? -m_dir : m_dir,
            .material = &m_material,
        },
        .dist = t,
        .hit = true
    };
}
