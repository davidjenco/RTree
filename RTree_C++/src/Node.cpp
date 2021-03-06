#include <iostream>
#include "Node.h"

using namespace std;

void Node::serializeNode(fstream &treeFileStream, const TreeConfig & config) {
    treeFileStream.write((char *) & id, sizeof(id));
    treeFileStream.write((char *) & isLeaf, sizeof(isLeaf));

    for (size_t i = 0; i < entries.size(); ++i) {
        entries[i]->serializeEntry(treeFileStream, isLeaf);
    }

    uint32_t maxEntries = config.maxNodeEntries;
    if (isLeaf){
        maxEntries = config.maxLeafNodeEntries;
    }

    if (entries.size() < maxEntries){
        RoutingEntry padding;

        padding.childNodeId = UINT32_MAX;
        padding.from.assign(config.dimension, INT32_MAX);
        padding.to.assign(config.dimension, INT32_MAX);

        for (size_t i = entries.size(); i < maxEntries; ++i) {
            padding.serializeEntry(treeFileStream, isLeaf);
        }
    }

    if (treeFileStream.fail()){
        treeFileStream.close();
        throw runtime_error("Error while writing to file (node)");
    }
}

void Node::readNode(fstream &treeFileStream, Node &node, const TreeConfig & config) {
    treeFileStream.read((char *) & node.id, sizeof(node.id));
    treeFileStream.read((char *) & node.isLeaf, sizeof(node.isLeaf));


    uint32_t maxEntries = config.maxNodeEntries;
    if (node.isLeaf){
        maxEntries = config.maxLeafNodeEntries;
    }

    for (int i = 0; i < maxEntries; ++i) {
        RoutingEntry routingEntry;
        RoutingEntry::readEntry(treeFileStream, routingEntry, node.isLeaf, config);
        if (routingEntry.childNodeId == UINT32_MAX){
            break;
        }
        else{
            node.entries.emplace_back(make_shared<RoutingEntry>(routingEntry));
        }
    }

    if (treeFileStream.fail()){
        treeFileStream.close();
        throw runtime_error("Error while reading from file (node)");
    }
}

Node & Node::rewriteEntry(shared_ptr<RoutingEntry> &routingEntry, const TreeConfig & config) {
    routingEntry->from.clear();
    routingEntry->to.clear();
//    routingEntry->from.resize(config.dimension);
//    routingEntry->to.resize(config.dimension);
    vector<int32_t> tmp;
    for (int i = 0; i < config.dimension; ++i) {
        tmp.clear();
        for (auto & entry : entries) {
            tmp.emplace_back(entry->from[i]);
            if (!isLeaf){
                tmp.emplace_back(entry->to[i]);
            }
        }
        size_t minIndex = min_element(tmp.begin(), tmp.end()) - tmp.begin();
        size_t maxIndex = max_element(tmp.begin(), tmp.end()) - tmp.begin();

        routingEntry->from.emplace_back(tmp[minIndex]);
        routingEntry->to.emplace_back(tmp[maxIndex]);
    }
    routingEntry->childNodeId = id; //not needed now, we'd say
    return *this;
}

void Node::rewriteNode(std::fstream & treeFileStream, const TreeConfig &config) {
    treeFileStream.seekp(config.metadataOffset + (id * config.nodeSizeInBytes), ios::beg);
    serializeNode(treeFileStream, config);
    treeFileStream.seekp(0, ios::end); //not needed now, we'd say

    if (treeFileStream.fail()){
        treeFileStream.close();
        throw runtime_error("Error while rewriting node");
    }
}

void Node::collectPoints(set<uint32_t> &result, const vector<int32_t> & searchFrom, const vector<int32_t> & searchTo) const{
    for (auto & entry : entries) {
        bool flag = true;
        for (size_t j = 0; j < searchFrom.size(); ++j) {
            if (searchFrom[j] > entry->from[j] || searchTo[j] < entry->from[j]){
                flag = false;
                break;
            }
        }
        if (flag)
            result.insert(entry->childNodeId);
    }
}

void Node::print(const TreeConfig &config) const{
    cout << "nodeID: " << id;
    if(isLeaf){
        printf(" Ground-Entries %lu/%d: \n", entries.size(), config.maxLeafNodeEntries);
        //for (size_t i = 0; i < entries.size(); ++i) {
        //    printf("\t\t%zu: ", i);
        //    for(auto & point: entries[i]->from)
        //        cout << point << " ";
        //    cout << endl;
        //}
    }else{
        printf(" Routing-Entries %lu/%d: \n", entries.size(), config.maxNodeEntries);
        for (const auto & entry : entries) {
            printf("\t\t%d: ", entry->childNodeId);
            for (size_t j = 0; j < entry->from.size(); ++j) {
                printf("<%d;%d> ", entry->from[j], entry->to[j]);
            }
            cout << endl;
        }
    }
}

double Node::calculateAreaIncrease(shared_ptr<RoutingEntry> &entryToBeIncreasedWith, const TreeConfig & config) {
    shared_ptr<RoutingEntry> originalNodeSurroundingEntry = make_shared<RoutingEntry>();
    rewriteEntry(originalNodeSurroundingEntry, config);

    double originalArea = originalNodeSurroundingEntry->calculateArea();
    double newArea = originalNodeSurroundingEntry->calculateAreaWithAnotherRoutingEntry(entryToBeIncreasedWith);

    return newArea - originalArea;

}
