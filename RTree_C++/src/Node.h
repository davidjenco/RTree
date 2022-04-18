#pragma once

#include <utility>
#include <vector>
#include "RoutingEntry.h"

///One node inside RTree - it's like container for RoutingEntries which in fact represents bounding box
struct Node{
    uint32_t id;
    bool isLeaf;
    std::vector<RoutingEntry> entries;

    Node(int id, bool isLeaf, std::vector<RoutingEntry> entries) : id(id), isLeaf(isLeaf), entries(std::move(entries)) {}

    Node() {}
};