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
    ///Explicit means that this constructor will be used only if it's explicitly typed
    explicit RTree(int dimension);

    ///Serializes metadata of the tree and also initialized root node (maybe will be modified in future)
    void serializeInit();

    ///Writes 32 uint in little endian
    void write32Uint(std::ofstream & outFile, uint32_t data);

    ///Reads 32 uint in little endian
    void read32Uint(std::ifstream & inFile, uint32_t & readUint);

    ///Calculates number of entries that can fit inside one node (depending on size of one node and dimension)
    uint32_t calculateMaxNodeEntries() const;

    ///Calculates number of entries that can fit inside one leaf node - it's always more, because
    ///leaf node entries need less memory/information (depending on size of one node and dimension)
    uint32_t calculateMaxLeafNodeEntries() const;

    void justTest();

};
