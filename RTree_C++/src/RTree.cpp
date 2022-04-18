#include <vector>
#include <iostream>
#include <map>
#include "RTree.h"
#include "InsertCandidate.h"

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
    config.serialize(treeOut);
    root.serializeNode(treeOut, config);

    if (treeOut.fail())
        throw runtime_error("Error while reading from file");

    treeOut.close();
}

void RTree::initStreams() {
    treeIn = ifstream(config.treeFileName, ios_base::binary);
    treeOut = ofstream(config.treeFileName, ios_base::binary);
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

void RTree::insert(const vector<int> & ranges) {
    if (ranges.size() != config.dimension)
        throw invalid_argument("Too few arguments for insert");

    treeIn.seekg(config.metadataOffset + config.rootId * config.nodeSizeInBytes);
    Node rootNode;
    Node::readNode(treeIn, rootNode, config);

    RecurseInsertStruct initParams;
    insertRec(treeIn, rootNode, ranges, initParams);

    treeIn.close();
}

void RTree::insertRec(std::ifstream & treeIn, const Node &node, const vector<int> &ranges, RecurseInsertStruct & params) {
    if (node.isLeaf){
        addIntoLeafNode(node, ranges, params);
        return;
    }

    vector<InsertCandidate> distances;
    for (const auto & entry : node.entries) {
        distances.emplace_back(entry.calculateDistance(ranges), entry.calculateArea());
    }
    auto min = min_element(distances.begin(), distances.end());
    RoutingEntry bestEntry = node.entries[min - distances.begin()];

    treeIn.seekg(config.metadataOffset + bestEntry.childNodeId * config.nodeSizeInBytes);
    Node childNode;
    Node::readNode(treeIn, childNode, config);

    insertRec(treeIn, childNode, ranges, params);

    //TODO continue here
    //update BestEntry mmb
    //split add to BestEntry
}

void RTree::addIntoLeafNode(const Node &leafNode, const vector<int> &ranges, RecurseInsertStruct & params) {
    if (leafNode.entries.size() == config.maxLeafNodeEntries){
        params.split = true;
        //TODO continue here
    }
}
