#pragma once

#include <utility>
#include <vector>
#include <set>
#include <memory>
#include "RoutingEntry.h"
#include "TreeConfig.h"

///One node inside RTree - it's like container for RoutingEntries which in fact represents bounding box
struct Node{

    uint32_t id;
    bool isLeaf;
    std::vector<std::shared_ptr<RoutingEntry>> entries;

    Node() = default;

    Node(uint32_t id, bool isLeaf, std::vector<std::shared_ptr<RoutingEntry>> entries) : id(id), isLeaf(isLeaf), entries(std::move(entries)) {}

    ///Serializes one node of the tree into binary file
    void serializeNode(std::fstream & treeFileStream, const TreeConfig & config);

    ///Rewrites node and sets put position in stream to end of this stream (we are always in end put position, just
    ///during rewriting we change it to demanded node position and then put it back on end)
    void rewriteNode(std::fstream & treeFileStream, const TreeConfig & config);

    ///Reads one node from binary file
    static void readNode(std::fstream & treeFileStream, Node & node, const TreeConfig & config);

    ///Rewrites RoutingEntry from node (so it means it always creates mbb, not point --> from and to is always set
    ///but depending on node type (leaf or no leaf) it calculates min max on certain dimension from either only from
    ///values (when leaf) or both from and to values (when no leaf))
    Node & rewriteEntry(std::shared_ptr<RoutingEntry> & routingEntry, const TreeConfig & config);

    ///Collect points from node that are within queried range (point's value on each dimension has to be among from-to
    ///range from query)
    void collectPoints(std::set<uint32_t> & result, const std::vector<int32_t> & searchFrom, const std::vector<int32_t> & searchTo) const;

    void print(const TreeConfig & config) const;
};