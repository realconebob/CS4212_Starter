#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <numbers>

#include "../src/helpers.h"
#include "../src/vecx.h"

const auto PI = std::numbers::pi;
const auto HALF_PI = std::numbers::pi / 2;
const auto FOURTH_PI = std::numbers::pi / 4;

auto v1 = Vec3D({0.1, 0.2, 0.3}),
        v2 = Vec3D({1.0, 2.0, 3.0}),
        v3 = Vec3D({10.0, 20.0, 30.0});

const double epsilon = 0.01;

TEST_CASE( "Generic Vector (VecX<double, 3>) Addition" ) {
    REQUIRE_DIFF((v1 + v2 + v3)[0], 11.1, epsilon);
    REQUIRE_DIFF((v1 + v2 + v3)[1], 22.2, epsilon);
    REQUIRE_DIFF((v1 + v2 + v3)[2], 33.3, epsilon);
}

TEST_CASE( "Generic Vector (VecX<double, 3>) Subtraction" ) {
    REQUIRE_DIFF((v1 - v2 - v3)[0], -10.9, epsilon);
    REQUIRE_DIFF((v1 - v2 - v3)[1], -21.8, epsilon);
    REQUIRE_DIFF((v1 - v2 - v3)[2], -32.7, epsilon);
}

TEST_CASE( "Generic Vector (VecX<double, 3>) Multiplication" ) {
    REQUIRE_DIFF((v1 * v2 * v3)[0], 1.0, epsilon);
    REQUIRE_DIFF((v1 * v2 * v3)[1], 8.0, epsilon);
    REQUIRE_DIFF((v1 * v2 * v3)[2], 27.0, epsilon);
}

TEST_CASE( "Generic Vector (VecX<double, 3>) Reverse Multiplication" ) {
    REQUIRE_DIFF((v3 * v2 * v1)[0], 1.0, epsilon);
    REQUIRE_DIFF((v3 * v2 * v1)[1], 8.0, epsilon);
    REQUIRE_DIFF((v3 * v2 * v1)[2], 27.0, epsilon);
}

TEST_CASE( "Generic Vector (VecX<double, 3>) Scalar Division" ) {
    REQUIRE_DIFF(v3[2] / 5, 6.0, epsilon);
    REQUIRE_DIFF(v2[2] / 5, 0.6, epsilon);
    REQUIRE_DIFF(v1[2] / 5, 0.06, epsilon);
}

TEST_CASE( "Generic Vector (VecX<double, 3>) Negation" ) {
    REQUIRE_DIFF((v1 + (-v1))[0], 0.0, epsilon);
    REQUIRE_DIFF((v2 + (-v2))[1], 0.0, epsilon);
    REQUIRE_DIFF((v3 + (-v3))[2], 0.0, epsilon);
}

TEST_CASE( "Generic Vector (VecX<double, 3>) +=" ) {
    REQUIRE_DIFF((Vec3D(v1.vals) += v2)[0], 1.1, epsilon);
    REQUIRE_DIFF((Vec3D(v2.vals) += v3)[1], 22.0, epsilon);
    REQUIRE_DIFF((Vec3D(v3.vals) += v1)[2], 30.3, epsilon);
}

TEST_CASE( "Generic Vector (VecX<double, 3>) Scalar *=" ) {
    REQUIRE_DIFF((Vec3D(v1.vals) *= 2)[0], 0.2, epsilon);
    REQUIRE_DIFF((Vec3D(v2.vals) *= 2)[1], 4.0, epsilon);
    REQUIRE_DIFF((Vec3D(v3.vals) *= 2)[2], 60.0, epsilon);
}

TEST_CASE( "Generic Vector (VecX<double, 3>) Scalar /=" ) {
    REQUIRE_DIFF((Vec3D(v1.vals) /= 2)[0], 0.05, epsilon);
    REQUIRE_DIFF((Vec3D(v2.vals) /= 2)[1], 1.0, epsilon);
    REQUIRE_DIFF((Vec3D(v3.vals) /= 2)[2], 15.0, epsilon);
}

TEST_CASE( "Generic Vector (VecX<double, 3>) Length Squared" ) {
    REQUIRE_DIFF(v1.length_squared(), 0.14, epsilon);
    REQUIRE_DIFF(v2.length_squared(), 14.0, epsilon);
    REQUIRE_DIFF(v3.length_squared(), 1400.0, epsilon);
}

TEST_CASE( "Generic Vector (VecX<double, 3>) Length" ) {
    REQUIRE_DIFF(v1.length(), 0.3742, epsilon);
    REQUIRE_DIFF(v2.length(), 3.742, epsilon);
    REQUIRE_DIFF(v3.length(), 37.42, epsilon);
}

TEST_CASE( "Generic Vector (VecX<double, 3>) Unit Vector" ) {
    REQUIRE_DIFF(unit(v1)[0], 0.269, epsilon);
    REQUIRE_DIFF(unit(v2)[1], 0.537, epsilon);
    REQUIRE_DIFF(unit(v3)[2], 0.800, epsilon);
}

TEST_CASE( "Generic Vector (VecX<double, 3>) Dot Product" ) {
    REQUIRE_DIFF(dot(v1, v2), 1.4, epsilon);
    REQUIRE_DIFF(dot(v2, v3), 140.0, epsilon);
    REQUIRE_DIFF(dot(v3, v1), 14.0, epsilon);
}

TEST_CASE( "Generic Vector (VecX<double, 3>) Reverse Dot Product" ) {
    REQUIRE_DIFF(dot(v2, v1), 1.4, epsilon);
    REQUIRE_DIFF(dot(v3, v2), 140.0, epsilon);
    REQUIRE_DIFF(dot(v1, v3), 14.0, epsilon);
}

TEST_CASE( "Generic Vector (VecX<double, 3>) Linear Transform" ) {
    REQUIRE_DIFF(linetr(v1, 0.1, 0.3, 0.0, 1.0)[0], 0.0, epsilon);
    REQUIRE_DIFF(linetr(v2, 1.0, 3.0, 0.0, 1.0)[1], 0.5, epsilon);
    REQUIRE_DIFF(linetr(v3, 10.0, 30.0, 0.0, 1.0)[2],1.0, epsilon);
}


TEST_CASE( "Generic Vector (VecX<double, 3>) Angle Between" ) {
    Vec3D
        zerodeg = Vec3D({1.0, 0.0, 0.0}),
        nindeydeg = Vec3D({0.0, 1.0, 0.0}),
        oneeightdeydeg = Vec3D({-1.0, 0.0, 0.0}),
        twosevendeydeg = Vec3D({0.0, -1.0, 0.0});

    // No difference between self
    REQUIRE_DIFF(anglbetw(zerodeg, zerodeg), 0, epsilon);
    REQUIRE_DIFF(anglbetw(nindeydeg, nindeydeg), 0, epsilon);
    REQUIRE_DIFF(anglbetw(oneeightdeydeg, oneeightdeydeg), 0, epsilon);
    REQUIRE_DIFF(anglbetw(twosevendeydeg, twosevendeydeg), 0, epsilon);

    // 90* differences
    REQUIRE_DIFF(anglbetw(zerodeg, nindeydeg), HALF_PI, epsilon);
    REQUIRE_DIFF(anglbetw(nindeydeg, oneeightdeydeg), HALF_PI, epsilon);
    REQUIRE_DIFF(anglbetw(oneeightdeydeg, twosevendeydeg), HALF_PI, epsilon);
    REQUIRE_DIFF(anglbetw(twosevendeydeg, zerodeg), HALF_PI, epsilon);

    // 180* differences
    REQUIRE_DIFF(anglbetw(zerodeg, oneeightdeydeg), PI, epsilon);
    REQUIRE_DIFF(anglbetw(nindeydeg, twosevendeydeg), PI, epsilon);
    REQUIRE_DIFF(anglbetw(oneeightdeydeg, zerodeg), PI, epsilon);
    REQUIRE_DIFF(anglbetw(twosevendeydeg, oneeightdeydeg), PI, epsilon);
}