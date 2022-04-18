#include <vector>
#include <iostream>
#include "RTree.h"

using namespace std;

RTree::RTree(int dimension) {
    root = Node(0, true, vector<RoutingEntry>());
    config.dimension = dimension;
    config.numberOfNodes++;
    config.rootId = 0;
    config.maxNodeEntries = calculateMaxNodeEntries();
    config.maxLeafNodeEntries = calculateMaxLeafNodeEntries();
}

void RTree::serializeInit() {
    ofstream treeOut (config.treeFileName, ios_base::binary);

    config.serialize(treeOut);
    root.serializeNode(treeOut, config);

    if (treeOut.fail())
        throw runtime_error("Error while reading from file");

    treeOut.close();
}

uint32_t RTree::calculateMaxNodeEntries() const{
    Node dummyNode;
    uint32_t result = config.nodeSizeInBytes - sizeof(dummyNode.id) - sizeof(dummyNode.isLeaf);
    //TODO kdyžtak jet ve for loop a postupně zvyšovat nodeSize pro větší dimenze
    uint32_t dummyEntrySize = 2 * (config.dimension * sizeof(int32_t)) + sizeof(uint32_t);
    return result / dummyEntrySize;
}


uint32_t RTree::calculateMaxLeafNodeEntries() const{
    Node dummyNode;
    uint32_t result = config.nodeSizeInBytes - sizeof(dummyNode.id) - sizeof(dummyNode.isLeaf);
    uint32_t dummyEntrySize = config.dimension * sizeof(int32_t) + sizeof(uint32_t);
    return result / dummyEntrySize;
}

void RTree::write32Uint(ofstream & outFile, uint32_t data) {
//    for (int i = 0; i < sizeof(data); ++i) {
//        char tmp = 0;
//        outFile.put(tmp | data >> i * 8);
//    }
    outFile.write( (char *) & data, sizeof(data) );
    if (outFile.fail())
        throw runtime_error("Error while writing to file");
}

void RTree::read32Uint(ifstream & inFile, uint32_t & readUint) {
    uint32_t result = 0;
//    for (int i = 0; i < sizeof(readUint); ++i) {
//        result |= ((unsigned char)inFile.get()) << i * 8;
//    }
    inFile.read( (char *) & result, sizeof(result) );
    if (inFile.fail())
        throw runtime_error("Error while reading from file");
    readUint = result;
}

void RTree::justTest() {
    ifstream ifs (config.treeFileName);
    uint32_t one, two, three, four, five, six;
    read32Uint(ifs, one);
    read32Uint(ifs, two);
    read32Uint(ifs, three);
    read32Uint(ifs, four);
    read32Uint(ifs, five);
    read32Uint(ifs, six);
    cout << one << " " << two << " " << three << " " << four << " " << five << " " << six << endl;
}
