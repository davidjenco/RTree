#include <iostream>
#include "Node.h"

using namespace std;

void Node::serializeNode(ofstream &treeOut, const TreeConfig & config) {
    treeOut.write((char *) & id, sizeof(id));
    treeOut.write((char *) & isLeaf, sizeof(isLeaf));

    for (size_t i = 0; i < entries.size(); ++i) {
        entries[i].serializeEntry(treeOut, isLeaf);
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
            padding.serializeEntry(treeOut, isLeaf);
        }
    }
}

void Node::readNode(ifstream &treeIn, Node &node, const TreeConfig & config) {
    treeIn.read((char *) & node.id, sizeof(node.id));
    treeIn.read((char *) & node.isLeaf, sizeof(node.isLeaf));

    uint32_t maxEntries = config.maxNodeEntries;
    if (node.isLeaf){
        maxEntries = config.maxLeafNodeEntries;
    }

    RoutingEntry routingEntry;
    for (int i = 0; i < maxEntries; ++i) {
        RoutingEntry::readEntry(treeIn, routingEntry, node.isLeaf, config);
        if (routingEntry.childNodeId == UINT32_MAX){
            break;
        }
        else{
            node.entries.push_back(routingEntry);
        }
    }
}

void Node::createEntry(RoutingEntry &routingEntry, const TreeConfig &config) {
    for (int i = 0; i < config.dimension; ++i) {
        vector<int32_t> tmp;
        for (auto & entry : entries) {
            tmp.emplace_back(entry.from[i]);
            if (!isLeaf){
                tmp.emplace_back(entry.to[i]);
            }
        }
        size_t minIndex = min_element(tmp.begin(), tmp.end()) - tmp.begin();
        size_t maxIndex = max_element(tmp.begin(), tmp.end()) - tmp.begin();

        routingEntry.from.emplace_back(tmp[minIndex]);
        routingEntry.to.emplace_back(tmp[maxIndex]);
    }
    routingEntry.childNodeId = id;
}

