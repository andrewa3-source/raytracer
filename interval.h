#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtweekend.h"


class interval {
  public:
    double min, max;

    //Default constructor sets interval to empty
    interval() : min(+infinity), max(-infinity) {} 
    //Constructor sets interval to [min, max]
    interval(double min, double max) : min(min), max(max) {}

    interval(const interval& a, const interval& b) {
        //Create inteal that encloses twi input intervals
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

    //Returns the size of the interval
    double size() const {
        return max - min;
    }

    //Returns boolean value based on whether the interval contains x
    //Inclusive
    bool contains(double x) const {
        return min <= x && x <= max;
    }

    //Returns boolean value based on whether the interval surrounds x
    //Exclusive
    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        return x < min ? min : x > max ? max : x;
    }

    interval expand(double delta) const {
        auto padding = delta/2;
        return interval(min - padding, max + padding);
    }

    static const interval empty, universe;
};

const interval interval::empty    = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif