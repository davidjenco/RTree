#include <vector>
#include <iostream>
#include <map>
#include <random>
#include <filesystem>
#include <queue>
#include "RTree.h"
#include "InsertCandidate.h"
#include "Splitter.h"

using namespace std;
using namespace std::filesystem;

RTree::RTree() {
    root = Node(0, true, vector<shared_ptr<RoutingEntry>>());
}

void RTree::configInit(uint32_t dimension) {
    config.dimension = dimension;
    config.numberOfNodes = 0;
    config.rootId = config.numberOfNodes++;

    for (; config.maxNodeEntries < config.minNodeEntries ; config.minPossibleNodeSize += 104) { //TODO change if needed
        config.nodeSizeInBytes = calculateNodeSize();
        config.maxNodeEntries = calculateMaxNodeEntries();
        config.maxLeafNodeEntries = calculateMaxLeafNodeEntries();
    }
    config.minPossibleNodeSize -= 104;
}

void RTree::serializeInit() {
    config.serialize(treeFileStream);
    root.serializeNode(treeFileStream, config);
}

uint32_t RTree::calculateNodeSize() const{
    Node dummyNode;

    uint32_t leafRoutingEntrySizeInSetDimension, noLeafRoutingEntrySizeInSetDimension;
    leafRoutingEntrySizeInSetDimension =  (config.dimension * sizeof(int32_t)) + sizeof(uint32_t);
    noLeafRoutingEntrySizeInSetDimension = 2 * (config.dimension * sizeof(int32_t)) + sizeof(uint32_t);

    double l = lcm(leafRoutingEntrySizeInSetDimension, noLeafRoutingEntrySizeInSetDimension);
    uint32_t minSizeForEntriesInNode = config.minPossibleNodeSize - (uint32_t)sizeof(dummyNode.id) - (uint32_t)sizeof(dummyNode.isLeaf);
    uint32_t k = ceil(minSizeForEntriesInNode / l);

    return k * (uint32_t) l + (uint32_t)sizeof(dummyNode.id) + (uint32_t)sizeof(dummyNode.isLeaf);
}

uint32_t RTree::calculateMaxNodeEntries() const{
    Node dummyNode;
    uint32_t result = config.nodeSizeInBytes - sizeof(dummyNode.id) - sizeof(dummyNode.isLeaf);
    uint32_t dummyEntrySize = 2 * (config.dimension * sizeof(int32_t)) + sizeof(uint32_t);
    return result / dummyEntrySize;
}

uint32_t RTree::calculateMaxLeafNodeEntries() const{
    Node dummyNode;
    uint32_t result = config.nodeSizeInBytes - sizeof(dummyNode.id) - sizeof(dummyNode.isLeaf);
    uint32_t dummyEntrySize = (config.dimension * sizeof(int32_t)) + sizeof(uint32_t);
    return result / dummyEntrySize;
}

void RTree::insert(const DataRow & data) {
    if (data.ranges.size() != config.dimension)
        throw invalid_argument("Too few arguments for insert");

    treeFileStream.seekg(config.metadataOffset + config.rootId * config.nodeSizeInBytes, ios::beg);
    Node rootNode;

    Node::readNode(treeFileStream, rootNode, config);

    //printf("\nRootNode read, rootID: %d number of entries: %lu/%d  while node size is %d\n"
    //       , rootNode.id, rootNode.entries.size(), config.maxNodeEntries, config.nodeSizeInBytes);
    //cout << endl << "RootNode. ";
    //rootNode.print(config);
    RecurseInsertStruct initParams;
    insertRec(rootNode, data, initParams);

    if (initParams.split){
        Node newRoot;
        newRoot.isLeaf = false;
        newRoot.id = config.numberOfNodes++;
        newRoot.entries.emplace_back(initParams.createdEntrySurroundingNewNodeIfSplit);
        shared_ptr<RoutingEntry> newEntrySurroundingOldRoot = make_shared<RoutingEntry>();
        rootNode.rewriteEntry(newEntrySurroundingOldRoot);
        newRoot.entries.emplace_back(newEntrySurroundingOldRoot);

        treeFileStream.seekp(0, ios::end);
        newRoot.serializeNode(treeFileStream, config);
        config.rootId = newRoot.id;
    }
}

void RTree::insertRec(Node &node, const DataRow & data, RecurseInsertStruct & params) {
    if (node.isLeaf){
        //cout << "Adding to leaf node. Row: ";
        //for(auto & i: data.ranges)
        //    cout << i << " ";
        //cout << endl;
        addIntoLeafNode(node, data, params);
        return;
    }
//    cout << "Choosing best entry in nodeID: " << node.id << endl;
    vector<InsertCandidate> distances;
    for (const auto & entry : node.entries) {
        distances.emplace_back(entry->calculateDistance(data.ranges), entry->calculateArea());
    }
    auto min = min_element(distances.begin(), distances.end());
    shared_ptr<RoutingEntry> bestEntry = node.entries[min - distances.begin()];

    treeFileStream.seekg(config.metadataOffset + bestEntry->childNodeId * config.nodeSizeInBytes, ios::beg);
//    cout << "Best entry found. Reading his child." << endl;
    Node childNode;
    Node::readNode(treeFileStream, childNode, config);
//    cout << "Best Entry Found. childNodeID: " << childNode.id << " isLeaf? --> " << boolalpha << childNode.isLeaf << endl;
//    cout << "childNode. ";
//    childNode.print(config);
    insertRec(childNode, data, params);

    if (params.split){
        if(node.entries.size() == config.maxNodeEntries){
            makeSplit(node, params.createdEntrySurroundingNewNodeIfSplit, params.createdEntrySurroundingNewNodeIfSplit);
        }else{
            params.split = false;
            node.entries.emplace_back(params.createdEntrySurroundingNewNodeIfSplit);
        }
        childNode.rewriteEntry(bestEntry); //enlarge bestEntry, params.enlarge stays true
        node.rewriteNode(treeFileStream, config); //child note is rewritten surely - whether leaf or used to be parent
    }
    else if (params.enlarged){ //means I have to check my mbb
        //determine if best entry needs to be enlarged and enlarge it if needed
        params.enlarged = bestEntry->enlargeEntry(data);
        if (params.enlarged){
            node.rewriteNode(treeFileStream, config); //could be optimized here maybe, just one entry changed
        }
    }
    //cout << "childNode---> Update ";
    //childNode.print(config);
}

void RTree::addIntoLeafNode(Node &leafNode, const DataRow & data, RecurseInsertStruct & params) {
//    cout << "Vector size : " << leafNode.entries.size();
//    cout << "  max entry in leaf node: " << config.maxLeafNodeEntries << endl;
    if (leafNode.entries.size() == config.maxLeafNodeEntries){
        params.split = true;
        params.enlarged = true;
//        cout << "Split in adding to leaf." << endl;
        makeSplit(leafNode, params.createdEntrySurroundingNewNodeIfSplit, make_shared<RoutingEntry>(RoutingEntry(data)));
    }
    else{
        params.split = false;
        params.enlarged = true;
//        cout << "No split in adding to leaf." << endl;
        leafNode.entries.emplace_back(make_shared<RoutingEntry>(RoutingEntry(data)));
    }
//    cout << "Writing leaf node. nodeID: " << leafNode.id << endl;
    leafNode.rewriteNode(treeFileStream, config);
}

void RTree::makeSplit(Node &fullNode, shared_ptr<RoutingEntry> &createdEntrySurroundingNewNode, const shared_ptr<RoutingEntry> &entryThatOverflowed) {
    //Create newNode and distribute entries between full and new node
    Node newNode1;
    newNode1.id = fullNode.id;
    newNode1.isLeaf = fullNode.isLeaf;
    Node newNode2;
    newNode2.id = config.numberOfNodes++;
    newNode2.isLeaf = fullNode.isLeaf;

    fullNode.entries.emplace_back(entryThatOverflowed);

    Splitter::makeQuadraticSplit(fullNode, newNode1, newNode2);

    fullNode = newNode1;

    //Serialize newNode
    treeFileStream.seekp(0, ios::end);
    newNode2.serializeNode(treeFileStream, config);

    createdEntrySurroundingNewNode = make_shared<RoutingEntry>();
    //With this code createdEntrySurroundingNewNodeIfSplit variable is changed
    newNode2.rewriteEntry(createdEntrySurroundingNewNode);
}

void RTree::initStreamsRecreateFile() {
    if (!exists("../res")){
        create_directories("../res");
    }

    treeFileStream.open(config.treeFileName, ios::in | ios::out | ios::binary | ios::trunc);
}

void RTree::initStreamsExistingFile() {
//    treeFileStream.open(config.treeFileName, ios::in | ios::out | ios::binary | ios::app);
    treeFileStream.open(config.treeFileName, ios::in | ios::out | ios::binary | ios::ate);
}

void RTree::closeStreams() {
    treeFileStream.close();
}

const TreeConfig &RTree::getConfig() const {
    return config;
}

const Node &RTree::getRoot() const {
    return root;
}

fstream &RTree::getTreeFileStream(){
    return treeFileStream;
}

void RTree::loadTree() {
    treeFileStream.seekg(0, ios::beg);
    config.readConfig(treeFileStream);

    config.maxNodeEntries = calculateMaxNodeEntries();
    config.maxLeafNodeEntries = calculateMaxLeafNodeEntries();
}

std::set<uint32_t> RTree::rangeSearch(const vector<int32_t> &searchFrom, const vector<int32_t> &searchTo) {
    set<uint32_t> result;

    shared_ptr<Node> rootNodePtr = cache.getNode(config.rootId, treeFileStream, config);
    rangeSearchRec(result, rootNodePtr, searchFrom, searchTo);

    return result;
}

void RTree::rangeSearchRec(set<uint32_t> &result, const shared_ptr<Node> & nodePtr, const vector<int32_t> &searchFrom,
                           const vector<int32_t> &searchTo) {
    if(nodePtr->isLeaf){
        nodePtr->collectPoints(result, searchFrom, searchTo);
        return;
    }
    else{
        for (auto & entry : nodePtr->entries) {
            if (entry->intersects(searchFrom, searchTo)){
                shared_ptr<Node> childNodePtr = cache.getNode(entry->childNodeId, treeFileStream, config);
                rangeSearchRec(result, childNodePtr, searchFrom, searchTo);
            }
        }
    }
}

void RTree::knnSearch(const vector<int32_t> &queryPoint, const size_t &k, set<KnnSearchStruct> &result) {
    priority_queue<KnnSearchStruct> qu;

    auto rootNodePtr = cache.getNode(config.rootId, treeFileStream, config);
    qu.push(KnnSearchStruct(rootNodePtr, 0));
    while (!qu.empty()){
        auto toSearch = qu.top();
        qu.pop();

        for (size_t i = 0; i < toSearch.node->entries.size(); ++i) {
            double distance = toSearch.node->entries[i]->calculateDistance(queryPoint);
            ///Inserting first K-Points
            if (result.size() < k){
                if (toSearch.node->isLeaf)
                    result.insert(KnnSearchStruct(toSearch.node->entries[i]->childNodeId, distance));
                else{
                    auto childNode = cache.getNode(toSearch.node->entries[i]->childNodeId, treeFileStream, config);
                    qu.push(KnnSearchStruct(childNode, distance));
                }
            ///Improving inserted K-points according to set max element (radius of search query)
            }else if(distance < result.rbegin()->distance){ ///discards Nodes that are too far

                if (toSearch.node->isLeaf){
                    result.insert(KnnSearchStruct(toSearch.node->entries[i]->childNodeId, distance));
                    result.erase(--result.end());
                }else{
                    auto childNode = cache.getNode(toSearch.node->entries[i]->childNodeId, treeFileStream, config);
                    qu.push(KnnSearchStruct(childNode, distance));
                }
            }
        }

    }
}

void RTree::saveConfig() {
    treeFileStream.seekp(0, ios::beg);
    config.serialize(treeFileStream);
}

