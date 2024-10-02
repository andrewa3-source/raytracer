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

    static const interval empty, universe;
};

const interval interval::empty    = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif