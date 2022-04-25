#pragma once

#include <cstdint>
#include <utility>
#include "Node.h"

///Represents struct for knn searching, that can be sorted based on its distance. For ground entries inserted in result
///set just (node) id is interesting for us, but for pushing into priority queue also ptr of Node is important, so struct
///is more general to satisfy both needs
struct KnnSearchStruct{
    std::shared_ptr<Node> node;
    double distance;

    KnnSearchStruct() = default;

    KnnSearchStruct(std::shared_ptr<Node> node, double distance) : node(std::move(node)), distance(distance) {}

    KnnSearchStruct(const uint32_t &id, double distance) : distance(distance) {
        node = std::make_shared<Node>();
        node->id = id;
    }

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