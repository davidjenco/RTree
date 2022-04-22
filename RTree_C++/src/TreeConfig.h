#pragma once

#include <string>
#include <fstream>

///Configuration structure for RTree (these hardcoded values will apply when creating whole new tree with generate (bulk load)
///but trees that are loaded from file will overwrite these values with their own values)
struct TreeConfig {
    std::string treeFileName = "../tree";
    int metadataOffset = 16;
    uint32_t numberOfNodes = 0;
    uint32_t minNodeEntries = 30; //TODO nastavit pak dynamicky podle dimenze a toho maxu
    uint32_t minPossibleNodeSize = 1029;
    uint32_t nodeSizeInBytes;
    uint32_t maxNodeEntries = 0;
    uint32_t maxLeafNodeEntries;
    uint32_t dimension;
    uint32_t rootId;

    TreeConfig() = default;

    ///Serializes metadata of the tree
    void serialize(std::fstream & treeFileStream);
};
