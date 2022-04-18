#pragma once

#include <string>

///Configuration structure for RTree (these hardcoded values will apply when creating whole new tree with generate (bulk load)
///but trees that are loaded from file will overwrite these values with their own values)
struct TreeConfig {
    std::string treeFileName = "../tree";
    int metadataOffset = 24;
    uint32_t numberOfEntries = 0;
    uint32_t nodeSizeInBytes = 2048;
    uint32_t minNodeEntries = 20;
    uint32_t maxNodeEntries = 50;
    uint32_t dimension;
    uint32_t rootId;

    TreeConfig() {}
};
