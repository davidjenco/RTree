#include <cmath>
#include <iostream>
#include "RoutingEntry.h"

using namespace std;

RoutingEntry::RoutingEntry(const DataRow &dataRow) : childNodeId(dataRow.rowId), from(dataRow.ranges) {}

void RoutingEntry::serializeEntry(fstream &treeFileStream, bool inLeafNode) {
    treeFileStream.write((char *) & childNodeId, sizeof(childNodeId));
    cout << "Serializing entry with id " << childNodeId << endl;
    for (size_t j = 0; j < from.size(); ++j) {
        treeFileStream.write((char *) & from[j], sizeof(from[j]));
        if (!inLeafNode){
            treeFileStream.write((char *) & to[j], sizeof(to[j]));
        }
    }
}

void RoutingEntry::readEntry(fstream &treeFileStream, RoutingEntry &routingEntry, bool inLeafNode, const TreeConfig & config) {
    treeFileStream.read((char *) & routingEntry.childNodeId, sizeof(routingEntry.childNodeId));


    int32_t range;
    for (size_t i = 0; i < config.dimension; ++i) {
        treeFileStream.read((char *) & range, sizeof(range));
        routingEntry.from.emplace_back(range);
        if (!inLeafNode){
            treeFileStream.read((char *) & range, sizeof(range));
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

bool RoutingEntry::enlargeEntry(const DataRow &point) {

    bool enlarged = false;
    for (size_t i = 0; i < from.size(); ++i) {
        if(from[i] > to[i])
            swap(from[i], to[i]);

        if (point.ranges[i] < from[i]){
            enlarged = true;
            from[i] = point.ranges[i];
        }else if (point.ranges[i] > to[i]){
            enlarged = true;
            to[i] = point.ranges[i];
        }
    }

    return enlarged;
}
