#include "Hittable.h"
#include "RayX.h"
#include "Sphere.h"
#include "vecx.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

auto sphere = Sphere3D<double>{Vec3D{0, 0, 0}, 1};

TEST_CASE("Sphere miss") {
    auto ray = Ray3D{Vec3D{1, 1, 1}, Vec3D{0, 0, -1}};
    REQUIRE_FALSE(sphere._intersect(ray));
}

TEST_CASE("Sphere tangent") {
    auto ray = Ray3D{Vec3D{1, 0, 0}, Vec3D{0, 0, -1}};
    REQUIRE(sphere._intersect(ray));
}

TEST_CASE("Sphere through") {
    auto ray = Ray3D{Vec3D{0, 0, 0}, Vec3D{0, 0, -1}};
    REQUIRE(sphere._intersect(ray));
}