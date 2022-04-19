#include <vector>
#include <iostream>
#include <map>
#include <random>
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

void RTree::insert(const DataRow & data) {
    if (data.ranges.size() != config.dimension)
        throw invalid_argument("Too few arguments for insert");

    treeIn.seekg(config.metadataOffset + config.rootId * config.nodeSizeInBytes);
    Node rootNode;
    Node::readNode(treeIn, rootNode, config);

    RecurseInsertStruct initParams;
    insertRec(rootNode, data, initParams);

    ///update split root?
    treeIn.close();
}

void RTree::insertRec(Node &node, const DataRow & data, RecurseInsertStruct & params) {
    if (node.isLeaf){
        addIntoLeafNode(node, data, params);
        return;
    }

    vector<InsertCandidate> distances;
    for (const auto & entry : node.entries) {
        distances.emplace_back(entry.calculateDistance(data.ranges), entry.calculateArea());
    }
    auto min = min_element(distances.begin(), distances.end());
    RoutingEntry bestEntry = node.entries[min - distances.begin()];

    treeIn.seekg(config.metadataOffset + bestEntry.childNodeId * config.nodeSizeInBytes);
    Node childNode;
    Node::readNode(treeIn, childNode, config);

    insertRec(childNode, data, params);

    //TODO continue here
    //update BestEntry mmb
    //split add to BestEntry
    if (params.split){
        if(node.entries.size() == config.maxNodeEntries){
            makeSplit(node, params.createdEntrySurroundingNewNodeIfSplit, params.createdEntrySurroundingNewNodeIfSplit);

        }else{
            params.split = false;
            node.entries.emplace_back(params.createdEntrySurroundingNewNodeIfSplit);

        }
        //TODO: BestEntry = new entry that fits changes childNode
    }
    if (params.enlarged){
        //TODO: Optimized update only for one entered entry
        if (!(to narostlo))
            enlarged = false;
    }

}

void RTree::addIntoLeafNode(Node &leafNode, const DataRow & data, RecurseInsertStruct & params) {
    if (leafNode.entries.size() == config.maxLeafNodeEntries){
        params.split = true;
        params.enlarged = true;

        makeSplit(leafNode, params.createdEntrySurroundingNewNodeIfSplit, RoutingEntry(data));
    }
    else{
        params.split = false;
        params.enlarged = true;

        leafNode.entries.emplace_back(RoutingEntry(data));
    }
}

void RTree::makeSplit(Node &fullNode, RoutingEntry &createdEntrySurroundingNewNode, const RoutingEntry &entryThatOverflowed) {
    //Create newNode and distribute entries between full and new node
    Node newNode;
    newNode.id = config.numberOfNodes++;
    newNode.isLeaf = fullNode.isLeaf;

    fullNode.entries.emplace_back(entryThatOverflowed);
    random_device rd;
    mt19937 rng(rd());
    shuffle(fullNode.entries.begin(), fullNode.entries.end(), rng);

    size_t const half_size = fullNode.entries.size() / 2;
    vector<RoutingEntry> half1(fullNode.entries.begin(), fullNode.entries.begin() + half_size);
    vector<RoutingEntry> half2(fullNode.entries.begin() + half_size, fullNode.entries.end());

    newNode.entries = half1;
    fullNode.entries = half2;

    //Serialize newNode
    newNode.serializeNode(treeOut, config);

    //With this code createdEntrySurroundingNewNodeIfSplit variable is changed
    newNode.createEntry(createdEntrySurroundingNewNode, config);
}
