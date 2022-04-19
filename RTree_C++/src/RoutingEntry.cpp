#include <cmath>
#include "RoutingEntry.h"

using namespace std;

RoutingEntry::RoutingEntry(const DataRow &dataRow) : childNodeId(dataRow.rowId), from(dataRow.ranges) {}

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

double RoutingEntry::calculateDistance(const vector<int> & ranges) const{

    vector<int> minDistance;
    for (size_t i = 0; i < from.size(); ++i) {
        int start = from[i];
        int end = to[i];
        if(start > end)
            swap(start, end);

        if(ranges[i] < start)
            minDistance.emplace_back(start - ranges[i]);
        else if(start <= ranges[i] && ranges[i] <= end)
            continue;
        else if(end < ranges[i])
            minDistance.emplace_back(ranges[i] - end);
    }
    int result = 0;
    for(auto & i : minDistance)
        result += i * i;
    return sqrt(result);
}

size_t RoutingEntry::calculateArea() const{

    vector<size_t> sideLength;
    size_t result = 1;
    for (size_t i = 0; i < from.size(); ++i)
        sideLength.emplace_back(abs(from[i] - to[i]));
    for(auto & i : sideLength)
        result *= i;
    return result;
}

void RoutingEntry::createFromNode(const Node &node, const TreeConfig & config) {
    for (int i = 0; i < config.dimension; ++i) {
        vector<int32_t> tmp;
        for (size_t j = 0; j < node.entries.size(); ++j) {
            tmp.emplace_back(node.entries.from[i]);
            if (!node.isLeaf){
                tmp.emplace_back(node.entries.to[i]);
            }
        }
        size_t minIndex = min_element(tmp.begin(), tmp.end()) - tmp.begin();
        size_t maxIndex = max_element(tmp.begin(), tmp.end()) - tmp.begin();

        from.emplace_back(tmp[minIndex]);
        to.emplace_back(tmp[maxIndex]);
    }
    childNodeId = node.id;
}
