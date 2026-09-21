#include "helpers.h"
#include <numbers>

double degtorad(double deg) {
    // 2rad = 360 = 1 rev, x deg * (2rad / 360deg) = x rad
    return deg * (2.0 * std::numbers::pi) / 360.0;
}