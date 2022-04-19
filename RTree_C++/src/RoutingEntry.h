#pragma once

#include <vector>
#include "DataRow.h"
#include "TreeConfig.h"

///Represents bounding box and contains also pointer to Node (content of this bounding box)
struct RoutingEntry{
    std::vector<int32_t> from;
    std::vector<int32_t> to;
    uint32_t childNodeId;

    RoutingEntry() = default;

    ///Creates new RoutingEntry from dataRow (so it means it always creates point, not mbb --> "to" property is empty)
    explicit RoutingEntry(const DataRow & dataRow);

    ///Serializes one entry of the node into binary file
    void serializeEntry(std::ofstream & treeOut, bool inLeafNode);

    ///Reads one entry from binary file
    static void readEntry(std::ifstream & treeIn, RoutingEntry & routingEntry, bool inLeafNode, const TreeConfig & config);

    ///Calculates distance of point (=ranges) from minimum bounding box
    double calculateDistance(const std::vector<int> & ranges) const;

    ///Calculates area of mbb
    size_t calculateArea() const;

    bool enlargeEntry(const DataRow & point);
};
