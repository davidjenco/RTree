#include <vector>
#include <iostream>
#include "RTree.h"

using namespace std;

RTree::RTree(int dimension) {
    root = Node(0, true, vector<RoutingEntry>());
    config.dimension = dimension;
    config.numberOfEntries++;
    config.rootId = 0;
}

void RTree::serializeInit() {
    ofstream treeOut (config.treeFileName);

    serializeMetadata(treeOut);
    serializeNode(treeOut, root);

    treeOut.close();
}

void RTree::serializeMetadata(ofstream & treeOut) {
    write32Uint(treeOut, config.numberOfEntries);
    write32Uint(treeOut, config.nodeSizeInBytes);
    write32Uint(treeOut, config.minNodeEntries);
    write32Uint(treeOut, config.maxNodeEntries);
    write32Uint(treeOut, config.dimension);
    write32Uint(treeOut, config.rootId);
}

void RTree::serializeNode(ofstream &treeOut, const Node &node) {
    write32Uint(treeOut, node.id);
    //TODO write isLeaf (bool, just byte)
    //TODO write empty vectors of routing entries depending on dimension
}

void RTree::write32Uint(ofstream &outFile, uint32_t data) {
    for (int i = 0; i < 4; ++i) {
        char tmp = 0;
        outFile.put(tmp | data >> i * 8);
    }
    if (outFile.fail())
        throw runtime_error("Error while writing to file");
}

void RTree::read32Uint(ifstream &inFile, uint32_t &readUint) {
    uint32_t result = 0;
    for (int i = 0; i < 4; ++i) {
        result |= ((unsigned char)inFile.get()) << i * 8;
    }
    if (inFile.fail())
        throw runtime_error("Error while reading from file");
    readUint = result;
}
