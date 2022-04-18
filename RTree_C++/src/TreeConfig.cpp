#include "TreeConfig.h"

void TreeConfig::serialize(std::ofstream &treeOut) {
    treeOut.write((char *) & numberOfNodes, sizeof(numberOfNodes));
    treeOut.write((char *) & nodeSizeInBytes, sizeof(nodeSizeInBytes));
    treeOut.write((char *) & dimension, sizeof(dimension));
    treeOut.write((char *) & rootId, sizeof(rootId));
}

