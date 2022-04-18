#include <fstream>
#include "RoutingEntry.h"

using namespace std;

void RoutingEntry::serializeEntry(ofstream &treeOut, bool inLeafNode) {
    treeOut.write((char *) & childNodeId, sizeof(childNodeId));
    for (size_t j = 0; j < from.size(); ++j) {
        treeOut.write((char *) & from[j], sizeof(from[j]));
        if (!inLeafNode){
            treeOut.write((char *) & to[j], sizeof(to[j]));
        }
    }
}

void RoutingEntry::readEntry(ifstream &treeIn, RoutingEntry &routingEntry, bool inLeafNode, const TreeConfig & config) {
    treeIn.read((char *) & routingEntry.childNodeId, sizeof(routingEntry.childNodeId));

    int range;
    for (size_t i = 0; i < config.dimension; ++i) {
        treeIn.read((char *) & range, sizeof(range));
        routingEntry.from.emplace_back(range);
        if (!inLeafNode){
            treeIn.read((char *) & range, sizeof(range));
            routingEntry.to.emplace_back(range);
        }
    }
}

