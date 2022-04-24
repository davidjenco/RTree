#pragma once

#include <cstdint>

struct KnnSearchStruct{
    uint32_t id;
    double distance;

    KnnSearchStruct() = default;

    KnnSearchStruct(const uint32_t &id, double distance) : id(id), distance(distance) {}

    bool operator<(const KnnSearchStruct &rhs) const {
        return distance < rhs.distance;
    }

    bool operator>(const KnnSearchStruct &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const KnnSearchStruct &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const KnnSearchStruct &rhs) const {
        return !(*this < rhs);
    }
};