#include "TreeConfig.h"

using namespace std;

void TreeConfig::serialize(ofstream &treeOut) {
    treeOut.write((char *) & numberOfNodes, sizeof(numberOfNodes));
    treeOut.write((char *) & nodeSizeInBytes, sizeof(nodeSizeInBytes));
    treeOut.write((char *) & dimension, sizeof(dimension));
    treeOut.write((char *) & rootId, sizeof(rootId));

    if (treeOut.fail())
        throw runtime_error("Error while reading from file");
}

