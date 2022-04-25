#pragma once

#include <vector>
#include <memory>
#include "Node.h"

class TreeCache {
private:
    std::vector<std::shared_ptr<Node>> cache;
    size_t capacity;

public:
    TreeCache() {
        capacity = 100;
        cache.assign(capacity, nullptr);
    }

    std::shared_ptr<Node> getNode(const uint32_t & nodeId, std::fstream & treeFileStream, const TreeConfig & config);
};


