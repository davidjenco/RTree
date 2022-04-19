#pragma once

#include <vector>
#include "Node.h"
#include "DataRow.h"

///Represents bounding box and contains also pointer to Node (content of this bounding box)
struct RoutingEntry{
    std::vector<int32_t> from;
    std::vector<int32_t> to;
    uint32_t childNodeId;

    RoutingEntry() = default;

    ///Creates new RoutingEntry from dataRow (so it means it always creates point, not mbb --> "to" property is empty)
    explicit RoutingEntry(const DataRow & dataRow);

    ///Creates new RoutingEntry from node (so it means it always creates mbb, not point --> from and to is always set
    ///but depending on node type (leaf or no leaf) it calculates min max on certain dimension from either only from
    ///values (when leaf) or both from and to values (when no leaf))
    void createFromNode(const Node & node, const TreeConfig & config);

    ///Serializes one entry of the node into binary file
    void serializeEntry(std::ofstream & treeOut, bool inLeafNode);

    ///Reads one entry from binary file
    static void readEntry(std::ifstream & treeIn, RoutingEntry & routingEntry, bool inLeafNode, const TreeConfig & config);

    ///Calculates distance of point (=ranges) from minimum bounding box
    double calculateDistance(const std::vector<int> & ranges) const;

    ///Calculates area of mbb
    size_t calculateArea() const;
};
