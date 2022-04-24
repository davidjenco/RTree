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

    ///Calculates distance of point (=point) from minimum bounding box (routing entry) or point (ground entry)
    double calculateDistance(const std::vector<int> & point) const;

    ///Calculates area of mbb
    size_t calculateArea() const;

    ///Checks whether routing entry ranges have to be enlarged depending on inserted point, if certain range needs to
    ///be enlarged in certain dimension it does enlarge (also returns true if one or more ranges enlarged, false otherwise)
    bool enlargeEntry(const DataRow & point);

    ///Checks whether routing entry intersects with queried ranges. It has to intersect it's intervals on every dimension.
    ///It's always invoked on routing entries (never on ground entries), so in one dimension it checks intervals
    ///intersect of routing entry's from-to and queried range from-to
    bool intersects(const std::vector<int32_t> & searchFrom, const std::vector<int32_t> & searchTo) const;

    ///Returns true if two sorted intervals intersects, false otherwise (intervals are always sorted, so first is
    ///always smaller than second in both intervals)
    static bool sortedIntervalsIntersect(const std::pair<int32_t, int32_t> & i1, const std::pair<int32_t, int32_t> & i2);
};
