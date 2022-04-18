#pragma once

#include <vector>
#include "TreeConfig.h"

///Represents bounding box and contains also pointer to Node (content of this bounding box)
struct RoutingEntry{
    std::vector<int32_t> from;
    std::vector<int32_t> to;
    uint32_t childNodeId;

    ///Serializes one entry of the node into binary file
    void serializeEntry(std::ofstream & treeOut, bool inLeafNode);

    ///Reads one entry from binary file
    static void readEntry(std::ifstream & treeIn, RoutingEntry & routingEntry, bool inLeafNode, const TreeConfig & config);
};