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

    Node() = default;

    Node(uint32_t id, bool isLeaf, std::vector<RoutingEntry> entries) : id(id), isLeaf(isLeaf), entries(std::move(entries)) {}

    ///Serializes one node of the tree into binary file
    void serializeNode(std::fstream & treeFileStream, const TreeConfig & config);

    ///Rewrites node and sets put position in stream to end of this stream (we are always in end put position, just
    ///during rewriting we change it to demanded node position and then put it back on end)
    void rewriteNode(std::fstream & treeFileStream, const TreeConfig & config);

    ///Reads one node from binary file
    static void readNode(std::fstream & treeFileStream, Node & node, const TreeConfig & config);

    ///Creates new RoutingEntry from node (so it means it always creates mbb, not point --> from and to is always set
    ///but depending on node type (leaf or no leaf) it calculates min max on certain dimension from either only from
    ///values (when leaf) or both from and to values (when no leaf))
    Node & createEntry(RoutingEntry & routingEntry, const TreeConfig & config);
};