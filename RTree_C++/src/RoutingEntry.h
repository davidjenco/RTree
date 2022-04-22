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
    void serializeEntry(std::fstream & treeFileStream, bool inLeafNode);

    ///Reads one entry from binary file
    static void readEntry(std::fstream & treeFileStream, RoutingEntry & routingEntry, bool inLeafNode, const TreeConfig & config);

    ///Calculates distance of point (=ranges) from minimum bounding box
    double calculateDistance(const std::vector<int> & ranges) const;

    ///Calculates area of mbb
    size_t calculateArea() const;

    bool enlargeEntry(const DataRow & point);

    bool intersects(const std::vector<int32_t> & searchFrom, const std::vector<int32_t> & searchTo) const;

    static bool sortedIntervalsIntersect(const std::pair<int32_t, int32_t> & i1, const std::pair<int32_t, int32_t> & i2);
};
