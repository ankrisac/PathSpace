#include "core.hpp"

Plane::Plane(Vec3 pos, Vec3 normal) {
    m_pos = pos;
    m_normal = normal;
}

void Plane::draw(Canvas& cnv) const {
    f32 d = glm::dot(m_pos, m_normal);
    Vec2 x = (d / (m_normal.x + m_normal.y)) * Vec2(m_normal); 
    Vec2 p = 2.0f * glm::normalize(Vec2(-p.y, p.x));
    cnv.line(x - p, x + p);
}

Hit Plane::intersect(Ray ray) const {
    f32 dN = glm::dot(ray.dir, m_normal);
    if (dN == 0.0) return {};

    f32 t = glm::dot(m_pos - ray.pos, m_normal) / dN;
    
    return {
        .pos = ray.pos + t * ray.dir,
        .normal = m_normal,
        .dist = t,
        .hit = true
    };
}