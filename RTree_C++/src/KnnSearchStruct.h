#pragma once

#include <cstdint>
#include <utility>
#include "Node.h"

///Represents struct for knn searching, that can be sorted based on its distance. For ground entries inserted in result
///set just (node) id is interesting for us, but for pushing into priority queue also ptr of Node is important, so struct
///is more general to satisfy both needs
struct KnnSearchStruct{
    double distance;
    std::shared_ptr<Node> node;

    KnnSearchStruct() = default;

    KnnSearchStruct(std::shared_ptr<Node> node, double distance) : node(std::move(node)), distance(distance) {}

    KnnSearchStruct(const uint32_t &id, double distance) : distance(distance) {
        node = std::make_shared<Node>();
        node->id = id;
    }

    bool operator<(const KnnSearchStruct &rhs) const {
        if (distance < rhs.distance)
            return true;
        if (rhs.distance < distance)
            return false;
        return node->id < rhs.node->id;
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

    bool operator==(const KnnSearchStruct &rhs) const {
        return (*this <= rhs) && (*this >= rhs);
    }

};