#include "catch2/catch_test_macros.hpp"
#include "cwrender/Helpers.hpp"
#include <limits>
#include <numbers>

using namespace cwrender;
auto const epsilon = std::numeric_limits<double>::epsilon();

TEST_CASE("Relative Diff") {
    REQUIRE(within_diff(relative_diff(5, 10), 1, epsilon));
    REQUIRE(within_diff(relative_diff(10, 5), 0.5, epsilon));
    REQUIRE(within_diff(relative_diff(10, 0), 1, epsilon));
    REQUIRE(within_diff(relative_diff(5, 0), 1, epsilon));
}

TEST_CASE("Swap") {
    auto a = 10, b = 5;
    REQUIRE(a == 10);
    REQUIRE(b == 5);

    swap(a, b);
    REQUIRE(b == 10);
    REQUIRE(a == 5);

    auto c = 15;
    swap(b, c);
    REQUIRE(a == 5);
    REQUIRE(b == 15);
    REQUIRE(c == 10);
}

TEST_CASE("Max, Min, Clamp") {
    auto a = 10, b = 15, c = 20;
    REQUIRE(min(a, b) == 10);
    REQUIRE(min(a, c) == 10);
    REQUIRE(min(b, c) == 15);

    REQUIRE(max(a, b) == 15);
    REQUIRE(max(a, c) == 20);
    REQUIRE(max(b, c) == 20);

    REQUIRE(clamp(a, b, c) == b);
    REQUIRE(clamp(b, a, c) == b);
    REQUIRE(clamp(c, a, b) == b);
    REQUIRE(clamp(-3, a, b) == a);
}

TEST_CASE("Degrees to Radians") {
    auto const
        fourtyfive = std::numbers::pi / 4,
        nintey = std::numbers::pi / 2,
        onethrityfive = fourtyfive * 3,
        oneeighty = std::numbers::pi,
        twotwentyfive = fourtyfive * 5,
        twoseventy = nintey * 3,
        threefifteen = fourtyfive * 7,
        threesixty = 2 * std::numbers::pi;

    REQUIRE_DIFF(degtorad(45), fourtyfive, epsilon);
    REQUIRE_DIFF(degtorad(90), nintey, epsilon);
    REQUIRE_DIFF(degtorad(135), onethrityfive, epsilon);
    REQUIRE_DIFF(degtorad(180), oneeighty, epsilon);
    REQUIRE_DIFF(degtorad(225), twotwentyfive, epsilon);
    REQUIRE_DIFF(degtorad(270), twoseventy, epsilon);
    REQUIRE_DIFF(degtorad(315), threefifteen, epsilon);
    REQUIRE_DIFF(degtorad(360), threesixty, epsilon);
}

TEST_CASE("Randomness") {
    auto const maxchecks = 1000;
    double r;
    for(int i = 0; i < maxchecks; i++) {
        r = zorand<double>();
        REQUIRE(r >= 0);
        REQUIRE(r <= 1);

        r = zorandr<double>(-10, 10);
        REQUIRE(r >= -10);
        REQUIRE(r <= 10);
    }
}
