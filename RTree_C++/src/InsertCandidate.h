#pragma once

#include <cstddef>

struct InsertCandidate {
    double distance;
    size_t area;

    InsertCandidate(double distance, const size_t &area) : distance(distance), area(area) {}

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