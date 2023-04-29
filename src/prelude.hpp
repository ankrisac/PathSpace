#pragma once

#include <cstdint>
#include <limits>

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

#include "math/vector.hpp"

using Float = f64;
using Vec2f = Vector2<Float>;
using Vec3f = Vector3<Float>;

using vec2  = Vector2<f32>;
using vec3  = Vector3<f32>;
using dvec2 = Vector2<f64>;
using dvec3 = Vector3<f64>;
using ivec2 = Vector2<i32>;
using ivec3 = Vector3<i32>;


struct Color : public ImplVector<Color, f64> {
    using Field = f64;
    Field r;
    Field g;
    Field b;

    static Color from(Vec3f value) {
        return Color { value.x, value.y, value.z };
    }

    static constexpr size_t len() {
        return 3;
    }
    Field& unsafe_get(size_t i) {
        return reinterpret_cast<Field*>(this)[i];
    }
    const Field& unsafe_get(size_t i) const {
        return reinterpret_cast<const Field*>(this)[i];
    }

    constexpr Color(Field gray): r(gray), g(gray), b(gray) {}
    constexpr Color(Field r, Field g, Field b): r(r), g(g), b(b) {}
};

namespace Colors {
    constexpr Color WHITE   = { 1.0, 1.0, 1.0 };
    constexpr Color BLACK   = { 0.0, 0.0, 0.0 };

    constexpr Color RED     = { 1.0, 0.0, 0.0 };
    constexpr Color GREEN   = { 0.0, 1.0, 0.0 };
    constexpr Color BLUE    = { 0.0, 0.0, 1.0 };

    constexpr Color CYAN    = { 0.0, 1.0, 1.0 };
    constexpr Color MAGENTA = { 1.0, 0.0, 1.0 };
    constexpr Color YELLOW  = { 1.0, 1.0, 0.0 };
};



#include <memory>
#include <vector>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>

inline void write_indent(std::ostream& out, size_t indent) {
    for(size_t i = 0; i < indent; i++) {
        out << "  ";
    }
}
inline std::string read_file(std::string path) {
    std::fstream file(path);

    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file [" << path << "]" << std::endl;
        std::abort();
    }
 
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}