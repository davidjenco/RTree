#pragma once

#include <string>
#include <fstream>

///Configuration structure for RTree (these hardcoded values will apply when creating whole new tree with generate (bulk load)
///but trees that are loaded from file will overwrite these values with their own values)
struct TreeConfig {
    std::string treeFileName = "../res/tree";
    int metadataOffset = 16;
    uint32_t minNodeEntries = 30;
    uint32_t minPossibleNodeSize = 240;
    uint32_t maxNodeEntries = 0;
    uint32_t maxLeafNodeEntries;

    ///In metadata
    uint32_t numberOfNodes;
    uint32_t nodeSizeInBytes;
    uint32_t dimension;
    uint32_t rootId;

    TreeConfig() = default;

    ///Serializes metadata of the tree
    void serialize(std::fstream & treeFileStream);

    ///Reads metadata of the tree
    void readConfig(std::fstream & treeFileStream);
};
