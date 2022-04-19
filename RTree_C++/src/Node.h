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

    ///Rewrites node and sets position in of stream to end of this stream
    void rewriteNode(std::ofstream & treeOut, const TreeConfig & config);

    ///Reads one node from binary file
    static void readNode(std::ifstream & treeIn, Node & node, const TreeConfig & config);

    ///Creates new RoutingEntry from node (so it means it always creates mbb, not point --> from and to is always set
    ///but depending on node type (leaf or no leaf) it calculates min max on certain dimension from either only from
    ///values (when leaf) or both from and to values (when no leaf))
    Node & createEntry(RoutingEntry & routingEntry, const TreeConfig & config);
};