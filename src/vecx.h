#ifndef CS4212_GRAPHICS_1283605022981__
#define CS4212_GRAPHICS_1283605022981__

#include <cmath>
#include <iostream>

#include <concepts>

template<typename T>
concept Floating = std::same_as<T, float> || std::same_as<T, double> || std::same_as<T, long double>;

template<Floating T, int X>
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
        constexpr int size() const { return X; }
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
template<Floating T, int X>
inline std::ostream& operator<<(std::ostream& out, const VecX<T, X>& v) {
        out << '(';
        for(int i = 0; i < X; i++) {
            out << v.vals[i];
            if(i + 1 < X) out << ' ';
        }
        return out << ')';
}

template<Floating T, int X>
inline VecX<T, X> operator+(const VecX<T, X>& a, const VecX<T, X>& b) {
    T res[X] = {};
    for(int i = 0; i < X; i++) {
        res[i] = a[i] + b[i];
    }
    return VecX<T, X>(res);
}

template<Floating T, int X>
inline VecX<T, X> operator-(const VecX<T, X>& a, const VecX<T, X>& b) {
    T res[X] = {};
    for(int i = 0; i < X; i++) {
        res[i] = a[i] - b[i];
    }
    return VecX<T, X>(res);
}

template<Floating T, int X>
inline VecX<T, X> operator*(const VecX<T, X>& a, const VecX<T, X>& b) {
    T res[X] = {};
    for(int i = 0; i < X; i++) {
        res[i] = a[i] * b[i];
    }
    return VecX<T, X>(res);
}

template<Floating T, int X>
inline VecX<T, X> operator*(T m, const VecX<T, X>& v) {
    T res[X] = {};
    for(int i = 0; i < X; i++) {
        res[i] = m * v[i];
    }
    return VecX<T, X>(res);
}
template<Floating T, int X>
inline VecX<T, X> operator*(const VecX<T, X>& v, T m) {
    return m * v;
}

template<Floating T, int X>
inline VecX<T, X> operator/(const VecX<T, X>& v, T m) {
    return (1/m) * v;
}

template<Floating T, int X>
inline T dot(const VecX<T, X>& a, const VecX<T, X>& b) {
    T sum = 0;
    for(int i = 0; i < X; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

// TODO: Implement generic cross product

template<Floating T, int X>
inline VecX<T, X> unit(const VecX<T, X>& v) {
    return v / v.length();
}

#pragma endregion

#endif