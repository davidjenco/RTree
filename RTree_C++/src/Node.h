#pragma once

#include <utility>
#include <vector>
#include "RoutingEntry.h"
#include "TreeConfig.h"

///One node inside RTree - it's like container for RoutingEntries which in fact represents bounding box
struct Node{

    uint32_t id;
    bool isLeaf;
    std::vector<RoutingEntry> entries;

    Node(uint32_t id, bool isLeaf, std::vector<RoutingEntry> entries) : id(id), isLeaf(isLeaf), entries(std::move(entries)) {}

    Node() = default;

    ///Serializes one node of the tree into binary file
    void serializeNode(std::ofstream & treeOut, const TreeConfig & config);

    ///Reads one node from binary file
    void readNode(std::ifstream & treeIn, Node & node, const TreeConfig & config);
};