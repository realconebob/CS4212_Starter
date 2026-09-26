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

#include "helpers.h"
#include <numbers>

double degtorad(double deg) {
    // 2rad = 360 = 1 rev, x deg * (2rad / 360deg) = x rad
    return deg * (2.0 * std::numbers::pi) / 360.0;
}