#include <iostream>
#include "TreeCache.h"

using namespace std;

shared_ptr<Node> TreeCache::getNode(const uint32_t &nodeId, fstream & treeFileStream, const TreeConfig & config) {
    auto cacheIndex = nodeId % capacity;
    if (cache.at(cacheIndex) != nullptr && cache.at(cacheIndex)->id == nodeId)
        return cache.at(cacheIndex);

    treeFileStream.seekg(config.metadataOffset + nodeId * config.nodeSizeInBytes, ios::beg);
    Node loadedNode;
    Node::readNode(treeFileStream, loadedNode, config);
    auto nodePtr = make_shared<Node>(loadedNode);

    cache[cacheIndex] = nodePtr;

    return nodePtr;
}

void TreeCache::init(const TreeConfig &config) {
    capacity = cacheSize / config.nodeSizeInBytes;
    cache.resize(capacity, nullptr);
    cache.shrink_to_fit();
}

void TreeCache::clearCache() {
    cache.clear();
}
