#pragma once

#include <string>
#include <fstream>
#include "Node.h"
#include "TreeConfig.h"
#include "RecurseInsertStruct.h"
#include "DataRow.h"

///RTree representation
class RTree {
private:
    Node root;
    TreeConfig config;
    std::ifstream treeIn;
    std::ofstream treeOut;

public:
    ///Explicit means that this constructor will be used only if it's explicitly typed
    explicit RTree(int dimension);

    ///Serializes metadata of the tree and also initialized root node (maybe will be modified in future)
    void serializeInit();

    ///Opens and assigns streams so reopening is not happening
    void initStreams();

    ///Calculates number of entries that can fit inside one node (depending on size of one node and dimension)
    uint32_t calculateMaxNodeEntries() const;

    ///Calculates number of entries that can fit inside one leaf node - it's always more, because
    ///leaf node entries need less memory/information (depending on size of one node and dimension)
    uint32_t calculateMaxLeafNodeEntries() const;

    void insert(const DataRow & data);

    void insertRec(Node & node, const DataRow & data, RecurseInsertStruct & params);

    void addIntoLeafNode(Node & leafNode, const DataRow & data, RecurseInsertStruct & params);

    void makeSplit(Node & fullNode, RoutingEntry & createdEntrySurroundingNewNode, const RoutingEntry & entryThatOverflowed);
};
