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

    ///Calculates number of entries that can fit inside one node (method depends on size of one node and dimension)
    uint32_t calculateMaxNodeEntries() const;

    ///Calculates number of entries that can fit inside one leaf node - it's always more, because
    ///leaf node entries need less memory/information (method depends on size of one node and dimension)
    uint32_t calculateMaxLeafNodeEntries() const;

    ///Inserts row of data into tree
    void insert(const DataRow & data);

    ///Traverses through tree into depth till leaf node is found and then inserts passed data to this node.
    ///On each level of the tree the best entry is chosen (the one which is the nearest to the point we are inserting).
    ///When we return from recurse, we may have to add potentially created entry needed for node splitting. Also
    ///we may need to update entry's ranges, because inserted point (data) could enlarge space needed for it's covering.
    ///All of this is done recursively, because both splitting nodes and enlarging entries can propagate up to root.
    void insertRec(Node & node, const DataRow & data, RecurseInsertStruct & params);

    void addIntoLeafNode(Node & leafNode, const DataRow & data, RecurseInsertStruct & params);

    ///Creates new node, splits entries between fulfilled node and this new node, serializes new node and also assign
    ///entry surrounding this new node (this entry is then inserted to the parent node entries)
    void makeSplit(Node & fullNode, RoutingEntry & createdEntrySurroundingNewNode, const RoutingEntry & entryThatOverflowed);
};
