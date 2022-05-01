#pragma once

#include <vector>
#include <memory>
#include "Node.h"

class Splitter {

public:
    static void quadraticSplit(Node & fullNode, Node & node1, Node & node2, const TreeConfig & config);

    ///Randomly distributes entries from fullNode into node1 and node2, exact half each
    static void randomSplit(Node & fullNode, Node & node1, Node & node2);

    ///Tests all pairs of entries to find the one pair with the biggest distance
    static std::pair<size_t, size_t> quadraticPickSeeds(Node & fullNode);

    ///Picks next Routing Entry to be inserted, picks the one with the biggest difference in area (distance)
    ///between two picked seeds
    static size_t pickNext(Node & fullNode, std::shared_ptr<RoutingEntry> & surroundingNode1,
                           std::shared_ptr<RoutingEntry> & surroundingNode2, const double areaNode1, const double areaNode2, const TreeConfig & config);

    ///Distributes Routing Entries using pickNext and inserting one by one, if one of the nodes reaches the half size of
    ///original fullNode, distributing stops and all  the rest Entries are assigned to other newNode
    static void quadraticDistribute(Node & fullNode, Node & node1, Node & node2, const size_t & halfNumOfNodes, const TreeConfig & config);
};


