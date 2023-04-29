#include "core.hpp"

Diamond& Diamond::with_material(Material value) {
    m_material = value;
    return *this;
}
Diamond& Diamond::with_pos(Vec3f value) {
    m_pos = value;
    return *this;
}
Diamond& Diamond::with_a(Vec3f value) {
    m_a = value;
    return *this;
}
Diamond& Diamond::with_b(Vec3f value) {
    m_b = value;
    return *this;
}

void Diamond::format(std::ostream& out, size_t indent) const {
    write_indent(out, indent);
    out << "Diamond(a: " << m_a << ", b: " << m_b << ")";
}

Intersection Diamond::intersect(Ray ray) const {    

    // Component of ray along plane normal
    Float ray_n = m_dir.dot(ray.dir);

    // Parallel to plane
    if(ray_n == 0.0) return { .hit = false };

    Float t = m_dir.dot(m_pos - ray.pos) / ray_n;
    if(t <= 0.0) return { .hit = false };

    Vec3f pos = ray.pos + ray.dir * t;

    Float mA = m_a.norm();
    Float mB = m_b.norm();

    Float a_comp = std::abs(m_a.dot(pos) / mA);
    Float b_comp = std::abs(m_b.dot(pos) / mB);

    // Outside diamond
    if(a_comp > mA || b_comp > mB) {
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