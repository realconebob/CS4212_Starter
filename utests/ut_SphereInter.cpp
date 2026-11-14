#include "cwrender/BaseTypes/VecX.hpp"
#include "cwrender/BaseTypes/RayX.hpp"
#include "cwrender/Hittables/Hittable.hpp"
#include "cwrender/Hittables/Sphere.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using namespace cwrender;
using Ray3D = RayX<double, 3>;

auto record = HitRecord<double, 3>{};
auto sphere = Sphere3<double>{Vec3D{0, 0, 0}, 1};
auto range = Interval<double>::universe();

TEST_CASE("Sphere miss") {
    auto ray = Ray3D{Vec3D{1, 1, 1}, Vec3D{0, 0, -1}};
    REQUIRE_FALSE(sphere.intersect(ray, record, range));
}

TEST_CASE("Sphere tangent") {
    auto ray = Ray3D{Vec3D{1, 0, 0}, Vec3D{0, 0, -1}};
    REQUIRE(sphere.intersect(ray, record, range));
}

TEST_CASE("Sphere through") {
    auto ray = Ray3D{Vec3D{0, 0, 0}, Vec3D{0, 0, -1}};
    REQUIRE(sphere.intersect(ray, record, range));
}