#include "core.hpp"

Sphere::Sphere(Vec3 pos, f32 radius) {
    m_pos = pos;
    m_radius = m_radius;
}

void Sphere::draw(Canvas& cnv) const {
    cnv.style.stroke = { 0.0, 0.0, 0.0, 1.0 };
    cnv.style.fill.a = 0.0;
    cnv.circle(m_pos, m_radius);
}

Hit Sphere::intersect(Ray ray) const {
    // Move sphere to origin
    Vec3 x = ray.pos - m_pos;

    f32 p = glm::dot(x, ray.dir);
    f32 q = glm::dot(x, x) - m_radius * m_radius;
    f32 D = p * p - q;

    if(q > 0) { // outside sphere
        if (D < 0) return {};

        f32 t = -p - std::sqrt(D);
        if (t < 0) return {};

        Vec3 y = x + t * ray.dir;

        return {
            .pos = y + m_pos,
            .normal = glm::normalize(y), 
            .dist = t,
            .hit = true
        };
    }
    else { // inside sphere
        f32 t = -p + std::sqrt(D);
        Vec3 y = x + t * ray.dir;

        return {
            .pos = y + m_pos,
            .normal = -glm::normalize(y),
            .dist = t,
            .hit = true
        };
    }
}