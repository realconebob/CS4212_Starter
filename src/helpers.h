#ifndef CS4212_GRAPHICS_HELPERS__14695208710861__
#define CS4212_GRAPHICS_HELPERS__14695208710861__

#define DEBUG 1

#ifdef DEBUG
#include <iostream>
#endif

#include <concepts>
#include <cmath>

template<typename T>
concept Floating = std::same_as<T, float> || std::same_as<T, double> || std::same_as<T, long double>;

template<typename T>
concept Numeric = std::integral<T> || Floating<T>;

template<typename T1, typename T2>
requires (std::convertible_to<T1, double>) && (std::convertible_to<T2, double>)
inline double relative_diff(const T1& v1, const T2& v2) {
    double
        dv1 = static_cast<double>(v1),
        dv2 = static_cast<double>(v2);

    return (v1 != 0)
        ? std::abs((dv2 - dv1 ) / dv1)
        : 0.0;
}

template<Numeric T1, Numeric T2>
inline bool within_diff(const T1& v1, const T2& v2, double maxdiff) {
    double res = relative_diff(v1, v2);

    #ifdef DEBUG
    std::cout << "Testing (" << v1 << ") against (" << v2 << ") with a diff epsilon of [" << maxdiff << "]: " << res << " <= " << maxdiff << " -> " << ((res <= maxdiff) ? "true" : "false") << "\n";
    #endif

    return res <= maxdiff;
}

#define REQUIRE_DIFF(v1, v2, max) REQUIRE(within_diff((v1), (v2), (max)))


template<typename T>
requires std::totally_ordered<T>
T floor(T val, T min) {
    val = (val < min) ? min : val;
    return val;
}

template<typename T>
requires std::totally_ordered<T>
T ceil(T val, T maxx) {
    val = (val > maxx) ? maxx : val;
    return val;
}

template<typename T>
requires std::totally_ordered<T>
T clamp(T val, T lo, T hi) {
    val = floor(val, lo);
    val = ceil(val, hi);
    return val;
}

#endif