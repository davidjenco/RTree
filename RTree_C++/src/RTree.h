#pragma once

#include <string>
#include <set>
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
    std::fstream treeFileStream;

public:
    RTree();

    ///Configures configuration data of the tree depending on dimension (mainly calling calculations methods)
    void configInit(int dimension);

    ///Serializes metadata of the tree and also initialized root node (maybe will be modified in future)
    void serializeInit();

    ///Calculates one node size in bytes depending on set dimension and minimum possible node size. It calculates
    ///lcm of ground entry and routing entry and then the multiplicative "k" that stands for the least multiplicative
    ///of lcm grater than minimum possible node size (ceil of division). This ensures that calculations
    ///"calculateMaxNodeEntries" and "calculateMaxLeafNodeEntries" have integer (not double) value
    uint32_t calculateNodeSize() const;

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

    ///Creates new file (or overwrites origin one)
    void initStreamsRecreateFile();

    ///Opens stream to existing file and sets cursor to end position with ate flag
    void initStreamsExistingFile();

    ///Closes stream
    void closeStreams();

    ///Loads tree metadata from existing file and does needed calculations to set tree configuration
    void loadTree();

    std::set<uint32_t> rangeSearch(const std::vector<int32_t> & searchFrom, const std::vector<int32_t> & searchTo);

    void rangeSearchRec(std::set<uint32_t> & result, const Node & node, const std::vector<int32_t> & searchFrom, const std::vector<int32_t> & searchTo);

    const TreeConfig &getConfig() const;

    const Node &getRoot() const;

    std::fstream &getTreeFileStream();
};
