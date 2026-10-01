/**
 * @file helpers.cpp
 * @author Connor Walstrom (walst110@umn.edu)
 * @brief Implementation of functions listed in helpers.h
 * @version 0.1
 * @date 2026-09-26
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "cwrender/Helpers.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <numbers>

namespace cwrender {

/// Initialize std::rand()'s randomness
void initrand() {
    static bool initialized = false;
    if(initialized) return;

    auto seed = std::time({});
    std::srand(seed);
    std::cout << "Randomness seed: " << seed << "\n";
    initialized = true;
    return;
}

double degtorad(double deg) {
    // 2rad = 360 = 1 rev, x deg * (2rad / 360deg) = x rad
    return deg * (std::numbers::pi) / 180.0;
}

}