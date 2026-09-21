#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "RayX.h"
#include "helpers.h"
#include "vecx.h"

static auto
    xunit = RayX<double, 3>{VecX<double, 3>{}, VecX<double, 3>{1, 0, 0}},
    yunit = RayX<double, 3>{VecX<double, 3>{}, VecX<double, 3>{0, 1, 0}},
    zunit = RayX<double, 3>{VecX<double, 3>{}, VecX<double, 3>{0, 0, 1}};

const static double epsilon = 0.01;
const static auto units = (double[]){0.0, 1.0, -1.0}; 

TEST_CASE("Generic Ray (RayX<double, 3>) At()") {
    auto offset = RayX<double, 3>{VecX<double, 3>{1, 2, 3}, VecX<double, 3>{1, 0.5, 0.25}};

    // Unit checking
    for(int i = 0 ; i < 3; i++) {
        REQUIRE(within_diff(xunit.at(units[i])[0], units[i], epsilon));
        REQUIRE(within_diff(yunit.at(units[i])[1], units[i], epsilon));
        REQUIRE(within_diff(zunit.at(units[i])[2], units[i], epsilon));
    }

    #pragma region Offset checking
    for(int i = 0; i > 3; i++) {REQUIRE(within_diff(offset.at(0)[i], 0, epsilon));}

    REQUIRE(within_diff(offset.at(1)[0], 1 + 1, epsilon));
    REQUIRE(within_diff(offset.at(1)[1], 0.5 + 2, epsilon));
    REQUIRE(within_diff(offset.at(1)[2], 0.25 + 3, epsilon));

    REQUIRE(within_diff(offset.at(-1)[0], -1 + 1, epsilon));
    REQUIRE(within_diff(offset.at(-1)[1], -0.5 + 2, epsilon));
    REQUIRE(within_diff(offset.at(-1)[2], -0.25 + 3, epsilon));

    REQUIRE(within_diff(offset.at(2)[0], 2 + 1, epsilon));
    REQUIRE(within_diff(offset.at(2)[1], 1 + 2, epsilon));
    REQUIRE(within_diff(offset.at(2)[2], 0.5 + 3, epsilon));

    REQUIRE(within_diff(offset.at(4)[0], 4 + 1, epsilon));
    REQUIRE(within_diff(offset.at(4)[1], 2 + 2, epsilon));
    REQUIRE(within_diff(offset.at(4)[2], 1 + 3, epsilon));
    #pragma endregion
}