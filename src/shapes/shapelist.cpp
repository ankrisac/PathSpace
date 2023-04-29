#include "core.hpp"

void ShapeList::format(std::ostream& out, size_t indent) const {
    write_indent(out, indent);
    out << "ShapeList[" << "\n";
    for(auto& shape : data) {
        shape->format(out, indent + 1);
        out << "\n";
    }
    write_indent(out, indent);
    out << "]\n";
}

Intersection ShapeList::intersect(Ray ray) const {
    Intersection nearest = { .dist = INFINITY }; 
    for(auto& shape : data) {
        Intersection current = shape->intersect(ray);
        if(current.dist < nearest.dist) {
            nearest = current;
        }
    }
    return nearest;
}
