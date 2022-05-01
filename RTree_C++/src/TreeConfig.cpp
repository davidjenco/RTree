#include "TreeConfig.h"

using namespace std;

void TreeConfig::serialize(fstream &treeFileStream) {
    treeFileStream.write((char *) & numberOfNodes, sizeof(numberOfNodes));
    treeFileStream.write((char *) & nodeSizeInBytes, sizeof(nodeSizeInBytes));
    treeFileStream.write((char *) & dimension, sizeof(dimension));
    treeFileStream.write((char *) & rootId, sizeof(rootId));

    if (treeFileStream.fail()){
        treeFileStream.close();
        throw runtime_error("Error while writing to file (metadata)");
    }
}

void TreeConfig::readConfig(fstream &treeFileStream) {
    treeFileStream.read((char *) & numberOfNodes, sizeof(numberOfNodes));
    treeFileStream.read((char *) & nodeSizeInBytes, sizeof(nodeSizeInBytes));
    treeFileStream.read((char *) & dimension, sizeof(dimension));
    treeFileStream.read((char *) & rootId, sizeof(rootId));

    if (treeFileStream.fail()){
        treeFileStream.close();
        throw runtime_error("Error while reading from file (metadata)");
    }
}


