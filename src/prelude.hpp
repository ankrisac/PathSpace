#pragma once

#include <vector>
#include <string>


#include <cstdint>
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

#include <numbers>
constexpr f32 PI = 3.14159265358979323846;

#include <glm/glm.hpp>
using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using Size = glm::ivec2;
using Color = glm::vec4;



template<typename T>
struct Grid2D {
    std::vector<T> data;
    Size size;

    Grid2D(Size size, T fill)
    : data(size.x * size.y, fill)
    , size(size) {} 

    bool contains(i32 i, i32 j) {
        return (0 <= i && i < size.x)
            && (0 <= j && j < size.y);
    }
    T& get(i32 i, i32 j) {
        return data[i + size.x * j];
    }
    const T& get(i32 i, i32 j) const {
        return data[i + size.x * j];
    }
};

#include <fmt/core.h>
