#pragma once

#include <string>
#include <fstream>
#include "Node.h"
#include "TreeConfig.h"

///RTree representation
///it's serialized in little endian
class RTree {
private:
    Node root;
    TreeConfig config;

public:
    RTree(int dimension);

    ///Serializes metadata of the tree and also initialized root node (maybe will be modified in future)
    void serializeInit();

    ///Serializes metadata of the tree
    void serializeMetadata(std::ofstream & treeOut);

    void serializeNode(std::ofstream & treeOut, const Node & node);

    ///Writes 32 uint in little endian
    void write32Uint(std::ofstream & outFile, uint32_t data);

    ///Reads 32 uint in little endian
    void read32Uint(std::ifstream & inFile, uint32_t & readUint);
};
