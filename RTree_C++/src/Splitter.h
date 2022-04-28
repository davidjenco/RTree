#pragma once

#include <vector>
#include <memory>
#include "Node.h"

class Splitter {

public:
    static void makeQuadraticSplit(Node & fullNode, Node & node1, Node & node2, const TreeConfig & config);

    static void randomSplit(Node & fullNode, Node & node1, Node & node2);

    static std::pair<size_t, size_t> quadraticPickSeeds(Node & fullNode);

    static size_t pickNext(Node & fullNode, Node & node1, Node & node2, const TreeConfig & config);

    static void quadraticDistribute(Node & fullNode, Node & node1, Node & node2, const size_t & halfNumOfNodes, const TreeConfig & config);

    static void quadraticDistributeForLeaf(Node & fullNode, Node & node1, Node & node2, const size_t & halfNumOfNodes);
};


