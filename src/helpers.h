#ifndef CS4212_GRAPHICS_HELPERS__14695208710861__
#define CS4212_GRAPHICS_HELPERS__14695208710861__

#include <concepts>
#include <cmath>

template<typename T>
concept Floating = std::same_as<T, float> || std::same_as<T, double> || std::same_as<T, long double>;

template<typename T>
concept Numeric = std::integral<T> || Floating<T>;

template<Numeric T1, Numeric T2>
inline bool withinDiff(T1 v1, T2 v2, double maxdiff) {
    double 
        dv1 = static_cast<double>(v1),
        dv2 = static_cast<double>(v2);
    
    double res = (v1 != 0)
        ? std::abs((v2 - v1 ) / v1)
        : 0.0;

    #ifdef DEBUG
    std::cout << "Testing (" << v1 << ") against (" << v2 << ") with a diff epsilon of [" << maxdiff << "]: " << res << " <= " << maxdiff << " -> " << ((res <= maxdiff) ? "true" : "false") << "\n";
    #endif

    return res <= maxdiff;
}

#define REQUIRE_DIFF(v1, v2, max) REQUIRE(withinDiff((v1), (v2), (max)))

#endif