/**
 * @file Interval.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2026-09-28
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef CS4212_GRAPHICS__INTERVAL__17666140626717__
#define CS4212_GRAPHICS__INTERVAL__17666140626717__

#include "Helpers.h"
#include <limits>
#include <sstream>
#include <stdexcept>

template<Floating T>
class Interval {
    public:
    T min, max;

    Interval(): min(-std::numeric_limits<T>::infinity()), max(std::numeric_limits<T>::infinity()) {}
    Interval(T lo, T hi): min(lo), max(hi) {
        if (hi < lo) {
            std::ostringstream err;
            err << "High bound (" << hi << ") is less than low bound (" << lo << ")";
            throw std::out_of_range(err.str());
        }
    }

    Interval operator-() const {return Interval<T>{-max, -min};}
    Interval& operator-() {swap(-min, -max); return *this;}

    T size() const {return max - min;}
    bool contains(T x) const {return min <= x && x <= max;}
    bool surrounds(T x) const {return min < x && x < max;}

    static Interval<T> empty() {return Interval<T>(std::numeric_limits<T>::infinity(), -std::numeric_limits<T>::infinity());}
    static Interval<T> universe() {return Interval<T>(-std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity());}
    static Interval<T> camera() {return Interval<T>(std::numeric_limits<T>::epsilon() * 10, std::numeric_limits<T>::infinity());}
};

#endif