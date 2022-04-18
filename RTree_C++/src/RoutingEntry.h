#pragma once

#include <vector>

///Represents bounding box and contains also pointer to Node (content of this bounding box)
struct RoutingEntry{
    std::vector<int> from;
    std::vector<int> to;
    uint childNodePtr;
};