/**
 * @file helpers.h
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief A collection of helper functions and definitions
 * @version 0.1
 * @date 2026-09-26
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS_HELPERS__14695208710861__
#define CS4212_GRAPHICS_HELPERS__14695208710861__

#include <cstddef>
#include <cstdlib>
#define DEBUG 1

#ifdef DEBUG
#include <iostream>
#endif

#include <concepts>
#include <cmath>

#define forever for(;;)

/**
 * @brief Concept describing the typical floating point number sizes
 *
 * @tparam T A type that satisfies std::same_as for `float`, `double`, or `long double`
 */
template<typename T>
concept Floating = std::same_as<T, float> || std::same_as<T, double> || std::same_as<T, long double>;

/**
 * @brief Concept describing most numbers
 *
 * @tparam T A type that satisfies `std::integral<T> || Floating<T>`
 */
template<typename T>
concept Numeric = std::integral<T> || Floating<T>;

class NotImplemented: public std::logic_error {
    public:
    NotImplemented(): std::logic_error("Function not implemented") {};
};

template<Floating T>
/// Returns a floating point between [0, 1]
T zorand() {
    return std::rand() / T(RAND_MAX);
}

template<typename T1, typename T2>
requires (std::convertible_to<T1, double>) && (std::convertible_to<T2, double>)
/**
 * @brief Calculate the relative difference between two values
 *
 * @param v1 Value 1
 * @param v2 Value 2
 * @return double How far value 2 is to value 1, as a multiple. 10's relative distance to 5 is 1, as 5 + (1*5) = 10
 */
inline double relative_diff(const T1& v1, const T2& v2) {
    double
        dv1 = static_cast<double>(v1),
        dv2 = static_cast<double>(v2);

    return (v1 != 0)
        ? std::abs((dv2 - dv1 ) / dv1)
        : 0.0;
}

template<Numeric T1, Numeric T2>
/**
 * @brief Tests whether two floating numbers are within some distance to each other
 *
 * @param v1 value 1
 * @param v2 value 2
 * @param maxdiff The max amount either value can differ from the other (inclusive). Measure is relative, so the distance between 10 and 5 is 1, and would satisfy <= 1
 * @return true
 * @return false
 */
inline bool within_diff(const T1& v1, const T2& v2, double maxdiff) {
    double res = relative_diff(v1, v2);

    #ifdef DEBUG
    std::cout << "Testing (" << v1 << ") against (" << v2 << ") with a diff epsilon of [" << maxdiff << "]: " << res << " <= " << maxdiff << " -> " << ((res <= maxdiff) ? "true" : "false") << "\n";
    #endif

    return res <= maxdiff;
}

#define REQUIRE_DIFF(v1, v2, max) REQUIRE(within_diff((v1), (v2), (max)))

template<Floating T>
inline T linear_to_gamma(T linear_comp) {
    if(linear_comp > 0) return std::sqrt(linear_comp);
    return 0;
}


template<typename T>
void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

template<typename T>
requires std::totally_ordered<T>
T min(T val, T min) {
    val = (val < min) ? min : val;
    return val;
}

template<typename T>
requires std::totally_ordered<T>
T max(T val, T maxx) {
    val = (val > maxx) ? maxx : val;
    return val;
}

template<typename T>
requires std::totally_ordered<T>
T clamp(T val, T lo, T hi) {
    val = min(val, lo);
    val = max(val, hi);
    return val;
}

double degtorad(double deg);


#endif