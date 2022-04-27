#pragma once

#include <cstddef>

struct InsertCandidate {
    double distance;
    double area;

    InsertCandidate(double distance, const double &area) : distance(distance), area(area) {}

    bool operator<(const InsertCandidate &rhs) const {
        if (distance < rhs.distance)
            return true;
        if (rhs.distance < distance)
            return false;
        return area < rhs.area;
    }

    bool operator>(const InsertCandidate &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const InsertCandidate &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const InsertCandidate &rhs) const {
        return !(*this < rhs);
    }
};