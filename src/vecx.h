#ifndef CS4212_GRAPHICS_VECX__1283605022981__
#define CS4212_GRAPHICS_VECX__1283605022981__

#include <cmath>
#include <cstddef>
#include <iostream>
#include <concepts>

template<typename T>
concept Floating = std::same_as<T, float> || std::same_as<T, double> || std::same_as<T, long double>;

template<Floating T, std::size_t X>
requires(X >= 1)
class VecX {
    public:
        #pragma region Variables
        T vals[X];
        #pragma endregion

        #pragma region Construtors
        VecX(): vals{} {}

        template<typename... Args>
        requires (sizeof...(Args) == X) && (std::same_as<Args, T> && ...) // Note: change == to <= if filling out everything becomes annoying. Array init rules mean the first x elements will be copied, and anything after will just be zero
        VecX(Args... args): vals{args...} {}

        explicit VecX(T (&args)[X]) {
            for(int i = 0; i < X; i++) {
                vals[i] = args[i];
            }
        }
        #pragma endregion

        #pragma region Operators
        VecX operator -() const {
            T copy[X] = {};
            for(int i = 0; i < X; i++) copy[i] = -vals[i];
            return VecX(copy);
        }

        T  operator [](int i) const {return vals[i];}
        T& operator [](int i) {return vals[i];}

        VecX& operator +=(const VecX& v) {
            for(int i = 0; i < X; i++) {
                vals[i] += v.vals[i];
            }
            return *this;
        }

        VecX& operator *=(T m) {
            for(int i = 0; i < X; i++) {
                vals[i] *= m;
            }
            return *this;
        }

        VecX& operator /=(T m) {
            return *this *= 1/m;
        }

        T length() const {
            return std::sqrt(length_squared());
        }

        T length_squared() const {
            T sum = 0;
            for(int i = 0; i < X; i++) {
                sum += vals[i] * vals[i];
            }
            return sum;
        }
        #pragma endregion

        #pragma region Misc
        constexpr std::size_t size() const { return X; }
        #pragma endregion
};

#pragma region Common Vector aliases
using Vec2F = VecX<float, 2>;
using Vec3F = VecX<float, 3>;
using Vec4F = VecX<float, 4>;

using Vec2D = VecX<double, 2>;
using Vec3D = VecX<double, 3>;
using Vec4D = VecX<double, 4>;

using Vec2LD = VecX<long double, 2>;
using Vec3LD = VecX<long double, 3>;
using Vec4LD = VecX<long double, 4>;
#pragma endregion

#pragma region Utility Functions
template<Floating T, std::size_t X>
inline std::ostream& operator<<(std::ostream& out, const VecX<T, X>& v) {
        out << '(';
        for(int i = 0; i < X; i++) {
            out << v.vals[i];
            if(i + 1 < X) out << ' ';
        }
        return out << ')';
}

template<Floating T, std::size_t X>
inline VecX<T, X> operator+(const VecX<T, X>& a, const VecX<T, X>& b) {
    T res[X] = {};
    for(int i = 0; i < X; i++) {
        res[i] = a[i] + b[i];
    }
    return VecX<T, X>(res);
}

template<Floating T, std::size_t X>
inline VecX<T, X> operator-(const VecX<T, X>& a, const VecX<T, X>& b) {
    T res[X] = {};
    for(int i = 0; i < X; i++) {
        res[i] = a[i] - b[i];
    }
    return VecX<T, X>(res);
}

template<Floating T, std::size_t X>
inline VecX<T, X> operator*(const VecX<T, X>& a, const VecX<T, X>& b) {
    T res[X] = {};
    for(int i = 0; i < X; i++) {
        res[i] = a[i] * b[i];
    }
    return VecX<T, X>(res);
}

template<Floating T, std::size_t X>
inline VecX<T, X> operator*(T m, const VecX<T, X>& v) {
    T res[X] = {};
    for(int i = 0; i < X; i++) {
        res[i] = m * v[i];
    }
    return VecX<T, X>(res);
}
template<Floating T, std::size_t X>
inline VecX<T, X> operator*(const VecX<T, X>& v, T m) {
    return m * v;
}

template<Floating T, std::size_t X>
inline VecX<T, X> operator/(const VecX<T, X>& v, T m) {
    return (1/m) * v;
}

template<Floating T, std::size_t X>
inline VecX<T, X> unit(const VecX<T, X>& v) {
    return v / v.length();
}

template<Floating T, std::size_t X>
inline T dot(const VecX<T, X>& a, const VecX<T, X>& b) {
    T sum = 0;
    for(int i = 0; i < X; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

template<Floating T, std::size_t X>
requires (X == 3)
inline VecX<T, X> cross(const VecX<T, X>& a, const VecX<T, X>& b) {
    T res[X] = {
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]
    };
    return VecX<T, X>(res);
}
// Cross product is specifically a 3d operation, so it's only defined here
// Ok technically you can generalize it but I don't think we're going to need an 8D cross product

template<Floating T, std::size_t X>
requires (X == 3)
inline VecX<T, X> normal(const VecX<T, X>& e1, const VecX<T, X>& e2) {
    return unit(cross(e1, e2));
}

template<Floating T, std::size_t X>
inline VecX<T, X> linetr(const VecX<T, X>& v, T oldl, T oldh, T newl, T newh) {
    // (x - min(x)) / (max(x) - min(x)) * (max(new) - min(new)) + min(new)
    T val = 0.0;
    T res[X] = {};
    for(int i = 0; i < X; i++) {
        val = v[i];
        res[i] = ((val * newh) + (-val * newl) + (-oldl * newh) + (oldl * newl)) / (oldh - oldl) + newl;
    }
    return VecX<T, X>(res);
}

template<Floating T, std::size_t X>
inline T anglbetw(const VecX<T, X>& a, const VecX<T, X>& b) {
    return acos(dot(a, b) / (a.length() * b.length()));
}

template<Floating T, std::size_t X>
inline VecX<T, X> dirto(const VecX<T, X>& a, const VecX<T, X>& b) {
    return unit(b - a);
}

#pragma endregion

#endif