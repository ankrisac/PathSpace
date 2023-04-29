#pragma once
#include <cstdint>
#include <cmath>
#include <ostream>

template<typename T> struct Vector2;
template<typename T> struct Vector3;

template<typename Vector> using Unit = Vector;

template<typename Vector, typename Field>
struct ImplVector {
    inline Vector& impl() {
        return *static_cast<Vector*>(this);
    }
    inline const Vector& impl() const {
        return *static_cast<const Vector*>(this);
    }

    Vector& operator+=(const Vector& rhs) {
        for(size_t i = 0; i < impl().len(); i++) {
            impl().unsafe_get(i) += rhs.unsafe_get(i);
        }
        return impl();
    }
    Vector& operator-=(const Vector& rhs) {
        for(size_t i = 0; i < impl().len(); i++) {
            impl().unsafe_get(i) -= rhs.unsafe_get(i);
        }
        return impl();
    }
    Vector& operator*=(const Vector& rhs) {
        for(size_t i = 0; i < impl().len(); i++) {
            impl().unsafe_get(i) *= rhs.unsafe_get(i);
        }
        return impl();
    }

    friend Vector operator+(Vector lhs, const Vector& rhs) {
        lhs += rhs;
        return lhs;
    }
    friend Vector operator-(Vector lhs, const Vector& rhs) {
        lhs -= rhs;
        return lhs;
    }
    friend Vector operator*(Vector lhs, const Vector& rhs) {
        lhs *= rhs;
        return lhs;
    }


    Vector& operator+=(const Field scalar) {
        for(size_t i = 0; i < impl().len(); i++) {
            impl().unsafe_get(i) += scalar;
        }
        return impl();
    }
    Vector& operator-=(const Field scalar) {
        for(size_t i = 0; i < impl().len(); i++) {
            impl().unsafe_get(i) -= scalar;
        }
        return impl();
    }
    Vector& operator*=(const Field scalar) {
        for(size_t i = 0; i < impl().len(); i++) {
            impl().unsafe_get(i) *= scalar;
        }
        return impl();
    }
    Vector& operator/=(const Field scalar) {
        for(size_t i = 0; i < impl().len(); i++) {
            impl().unsafe_get(i) /= scalar;
        }
        return impl();
    }

    friend Vector operator*(Vector vec, Field scalar) {
        vec *= scalar;
        return vec;
    }
    friend Vector operator/(Vector vec, Field scalar) {
        vec /= scalar;
        return vec;
    }
    friend Vector operator*(Field scalar, Vector vec) {
        vec *= scalar;
        return vec;
    }

    Vector reverse() const {
        Vector out;
        for(size_t i = 0; i < impl().len(); i++) {
            out.unsafe_get(i) = -impl().unsafe_get(i);
        }
        return out;
    }
    Vector operator-() const {
        return reverse();
    }

    Field dot(const Vector& rhs) const {
        Field sum = Field(0);
        for(size_t i = 0; i < impl().len(); i++) {
            sum += impl().unsafe_get(i) * rhs.unsafe_get(i);
        }
        return sum;
    }

    Field norm_squared() const {
        return dot(impl());
    }
    Field norm() const {
        return std::sqrt(dot(impl()));
    }

    Vector& normalize() {
        *this *= (Field(1) / norm());
        return impl();
    }
    Unit<Vector> unit() const {
        Unit<Vector> unit = impl();
        unit.normalize();
        return unit;
    }

    Vector project(const Vector& component) const {
        return component * component.dot(impl());
    }

    Vector facing(const Vector& direction) const {
        if (this->dot(direction) >= Field(0.0)) {
            return impl();
        }
        return reverse();
    }
};


template<typename T> 
struct Vector2 : public ImplVector<Vector2<T>, T> {
    T x;
    T y;

    Vector2(): x(0), y(0) {}
    Vector2(T x, T y): x(x), y(y) {}

    static constexpr size_t len() {
        return 2;
    }
    T& unsafe_get(size_t i) {
        return reinterpret_cast<T*>(this)[i];
    }
    const T& unsafe_get(size_t i) const {
        return reinterpret_cast<const T*>(this)[i];
    }

    static Vector2 splat(T value) {
        return { value, value };
    }
    static Vector2 polar(T angle, T radius = T(1)) {
        return { 
            radius * std::cos(angle), 
            radius * std::sin(angle) 
        };
    }
    Vector3<T> with_z(T z) const {
        return { x, y, z };
    }

    template<typename U>
    Vector2<U> cast() const {
        return {
            static_cast<U>(x),
            static_cast<U>(y)
        };
    }

    /// Rotate via complex multiplication
    /// ie: add angle, multiply length
    Vector2 rotate(Vector2 rhs) const {
        return {
            x * rhs.x - y * rhs.y, 
            x * rhs.y + y * rhs.x
        };
    }
    Vector2 perp() const {
        return { -y, x };
    }


    friend std::ostream& operator<<(std::ostream& out, const Vector2<T>& vec) {
        return out << "["
            << vec.x << ","
            << vec.y << "]"; 
    }
};

template<typename T> 
struct Vector3 : public ImplVector<Vector3<T>, T> {
    T x;
    T y;
    T z;

    Vector3(): x(0), y(0), z(0) {}
    Vector3(T x, T y, T z): x(x), y(y), z(z) {}

    static consteval size_t len() {
        return 3;
    }
    T& unsafe_get(size_t i) {
        return reinterpret_cast<T*>(this)[i];
    }
    const T& unsafe_get(size_t i) const {
        return reinterpret_cast<const T*>(this)[i];
    }

    static Vector3 splat(T value) {
        return { value, value, value };
    }

    Vector2<T> xy() const { return { x, y }; }
    Vector2<T> xz() const { return { x, z }; }
    Vector2<T> yz() const { return { y, z }; }

    template<typename U>
    Vector3<U> cast() const {
        return {
            static_cast<U>(x),
            static_cast<U>(y),
            static_cast<U>(z)
        };
    }

    Vector3 cross(const Vector3& rhs) const {
        return {
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x
        };
    }

    friend std::ostream& operator<<(std::ostream& out, const Vector3<T>& vec) {
        return out << "["
            << vec.x << ","
            << vec.y << ","
            << vec.z << "]"; 
    }

};

template<typename T>
Vector3<T> reflect(Vector3<T> I, Vector3<T> N) {
    return I - I.project(N) * 2.0f;
}

template<typename T>
Vector3<T> refract(Vector3<T> I, Vector3<T> N, T mu) {
    auto k = 1.0f - mu * mu * (1.0f - I.dot(N));

    if (k < 0.0f) {
        return Vector3<T>::splat(0.0f);
    }
    else {
        return I * mu  - N * mu * (N.dot(I) + std::sqrt(k));
    }
}
