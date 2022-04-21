#include "TreeConfig.h"

using namespace std;

void TreeConfig::serialize(fstream &treeFileStream) {
    treeFileStream.write((char *) & numberOfNodes, sizeof(numberOfNodes));
    treeFileStream.write((char *) & nodeSizeInBytes, sizeof(nodeSizeInBytes));
    treeFileStream.write((char *) & dimension, sizeof(dimension));
    treeFileStream.write((char *) & rootId, sizeof(rootId));

    if (treeFileStream.fail())
        throw runtime_error("Error while writing to file (metadata)");
}

