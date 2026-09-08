#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../src/vecx.h"
#include <cmath>

template<Floating T>
inline bool withinDiff(T v1, T v2, double maxdiff) {
    T res = (v1 != 0)
        ? std::abs((v2 - v1 ) / v1)
        : 0.0;

    std::cout << "Testing (" << v1 << ") against (" << v2 << ") with a diff epsilon of [" << maxdiff << "]: " << res << " <= " << maxdiff << " -> " << ((res <= maxdiff) ? "true" : "false") << "\n";
    return res <= maxdiff;
}

auto v1 = Vec3D({0.1, 0.2, 0.3}),
        v2 = Vec3D({1.0, 2.0, 3.0}),
        v3 = Vec3D({10.0, 20.0, 30.0});

const double epsilon = 0.01;

TEST_CASE( "Generic Vector Addition" ) {
    REQUIRE(withinDiff((v1 + v2 + v3)[0], 11.1, epsilon));
    REQUIRE(withinDiff((v1 + v2 + v3)[1], 22.2, epsilon));
    REQUIRE(withinDiff((v1 + v2 + v3)[2], 33.3, epsilon));
}

TEST_CASE( "Generic Vector Subtraction" ) {
    REQUIRE(withinDiff((v1 - v2 - v3)[0], -10.9, epsilon));
    REQUIRE(withinDiff((v1 - v2 - v3)[1], -21.8, epsilon));
    REQUIRE(withinDiff((v1 - v2 - v3)[2], -32.7, epsilon));
}

TEST_CASE( "Generic Vector Multiplication" ) {
    REQUIRE(withinDiff((v1 * v2 * v3)[0], 1.0, epsilon));
    REQUIRE(withinDiff((v1 * v2 * v3)[1], 8.0, epsilon));
    REQUIRE(withinDiff((v1 * v2 * v3)[2], 27.0, epsilon));
}

TEST_CASE( "Generic Vector Reverse Multiplication" ) {
    REQUIRE(withinDiff((v3 * v2 * v1)[0], 1.0, epsilon));
    REQUIRE(withinDiff((v3 * v2 * v1)[1], 8.0, epsilon));
    REQUIRE(withinDiff((v3 * v2 * v1)[2], 27.0, epsilon));
}

TEST_CASE( "Generic Vector Scalar Division" ) {
    REQUIRE(withinDiff(v3[2] / 5, 6.0, epsilon));
    REQUIRE(withinDiff(v2[2] / 5, 0.6, epsilon));
    REQUIRE(withinDiff(v1[2] / 5, 0.06, epsilon));
}

TEST_CASE( "Generic Vector Negation" ) {
    REQUIRE(withinDiff((v1 + (-v1))[0], 0.0, epsilon));
    REQUIRE(withinDiff((v2 + (-v2))[1], 0.0, epsilon));
    REQUIRE(withinDiff((v3 + (-v3))[2], 0.0, epsilon));
}

TEST_CASE( "Generic Vector +=" ) {
    REQUIRE(withinDiff((Vec3D(v1.vals) += v2)[0], 1.1, epsilon));
    REQUIRE(withinDiff((Vec3D(v2.vals) += v3)[1], 22.0, epsilon));
    REQUIRE(withinDiff((Vec3D(v3.vals) += v1)[2], 30.3, epsilon));
}

TEST_CASE( "Generic Vector Scalar *=" ) {
    REQUIRE(withinDiff((Vec3D(v1.vals) *= 2)[0], 0.2, epsilon));
    REQUIRE(withinDiff((Vec3D(v2.vals) *= 2)[1], 4.0, epsilon));
    REQUIRE(withinDiff((Vec3D(v3.vals) *= 2)[2], 60.0, epsilon));
}

TEST_CASE( "Generic Vector Scalar /=" ) {
    REQUIRE(withinDiff((Vec3D(v1.vals) /= 2)[0], 0.05, epsilon));
    REQUIRE(withinDiff((Vec3D(v2.vals) /= 2)[1], 1.0, epsilon));
    REQUIRE(withinDiff((Vec3D(v3.vals) /= 2)[2], 15.0, epsilon));
}

TEST_CASE( "Generic Vector Length Squared" ) {
    REQUIRE(withinDiff(v1.length_squared(), 0.14, epsilon));
    REQUIRE(withinDiff(v2.length_squared(), 14.0, epsilon));
    REQUIRE(withinDiff(v3.length_squared(), 1400.0, epsilon));
}

TEST_CASE( "Generic Vector Length" ) {
    REQUIRE(withinDiff(v1.length(), 0.3742, epsilon));
    REQUIRE(withinDiff(v2.length(), 3.742, epsilon));
    REQUIRE(withinDiff(v3.length(), 37.42, epsilon));
}

TEST_CASE( "Generic Vector Unit Vector" ) {
    REQUIRE(withinDiff(unit(v1)[0], 0.269, epsilon));
    REQUIRE(withinDiff(unit(v2)[1], 0.537, epsilon));
    REQUIRE(withinDiff(unit(v3)[2], 0.800, epsilon));
}

TEST_CASE( "Generic Vector Dot Product" ) {
    REQUIRE(withinDiff(dot(v1, v2), 1.4, epsilon));
    REQUIRE(withinDiff(dot(v2, v3), 140.0, epsilon));
    REQUIRE(withinDiff(dot(v3, v1), 14.0, epsilon));
}

TEST_CASE( "Generic Vector Reverse Dot Product" ) {
    REQUIRE(withinDiff(dot(v2, v1), 1.4, epsilon));
    REQUIRE(withinDiff(dot(v3, v2), 140.0, epsilon));
    REQUIRE(withinDiff(dot(v1, v3), 14.0, epsilon));
}

TEST_CASE( "Generic Vector Linear Transform" ) {
    REQUIRE(withinDiff(linetr(v1, 0.1, 0.3, 0.0, 1.0)[0], 0.0, epsilon));
    REQUIRE(withinDiff(linetr(v2, 1.0, 3.0, 0.0, 1.0)[1], 0.5, epsilon));
    REQUIRE(withinDiff(linetr(v3, 10.0, 30.0, 0.0, 1.0)[2],1.0, epsilon));
}

/*
TEST_CASE( "Generic Vector Angle Between" ) {
    REQUIRE(withinDiff(v1.length(), 0.3742, epsilon));
    REQUIRE(withinDiff(v2.length(), 3.742, epsilon));
    REQUIRE(withinDiff(v3.length(), 37.42, epsilon));
}

TEST_CASE( "Generic Vector Direction To" ) {
    REQUIRE(withinDiff(v1.length(), 0.3742, epsilon));
    REQUIRE(withinDiff(v2.length(), 3.742, epsilon));
    REQUIRE(withinDiff(v3.length(), 37.42, epsilon));
}
*/