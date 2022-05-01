#pragma once

#include <vector>
#include <memory>
#include "Node.h"

///Represents cache of the tree for nodes, that can be stored in memory and be accesses quicker than while reading
///from disk
class TreeCache {
private:
    std::vector<std::shared_ptr<Node>> cache;
    size_t capacity;
    size_t cacheSize = 1e6; //1 MB

public:
    TreeCache() = default;

    ///Reads node with given nodeId, may be already stored in vector, in that case returns straight forward, otherwise
    ///read node from "tree" binary file and save it to vector
    std::shared_ptr<Node> getNode(const uint32_t & nodeId, std::fstream & treeFileStream, const TreeConfig & config);

    ///Counts cache capacity and resizes vector with nulls
    void init(const TreeConfig & config);

    ///Clears the cache's vector
    void clearCache();
};


