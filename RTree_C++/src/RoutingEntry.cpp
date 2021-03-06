#include <cmath>
#include <iostream>
#include "RoutingEntry.h"

using namespace std;

RoutingEntry::RoutingEntry(const DataRow &dataRow) : childNodeId(dataRow.rowId), from(dataRow.ranges) {}

void RoutingEntry::serializeEntry(fstream &treeFileStream, bool inLeafNode) {
    treeFileStream.write((char *) & childNodeId, sizeof(childNodeId));
    for (size_t j = 0; j < from.size(); ++j) {
        treeFileStream.write((char *) & from[j], sizeof(from[j]));
        if (!inLeafNode){
            treeFileStream.write((char *) & to[j], sizeof(to[j]));
        }
    }
}

void RoutingEntry::readEntry(fstream &treeFileStream, RoutingEntry &routingEntry, bool inLeafNode, const TreeConfig & config) {
    treeFileStream.read((char *) & routingEntry.childNodeId, sizeof(routingEntry.childNodeId));

    routingEntry.from.reserve(config.dimension);
    routingEntry.to.reserve(config.dimension);
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

double RoutingEntry::calculateDistance(const vector<int> & point) const{

    vector<int> minDistance;
    minDistance.reserve(from.size());
    if (from.size() == to.size()){
        for (size_t i = 0; i < from.size(); ++i) {
            int start = from[i];
            int end = to[i];
            if(start > end)
                swap(start, end);

            if(point[i] < start)
                minDistance.emplace_back(start - point[i]);
            else if(start <= point[i] && point[i] <= end)
                continue;
            else if(end < point[i])
                minDistance.emplace_back(point[i] - end);
        }
    }else{
        for (size_t i = 0; i < from.size(); ++i) {
            minDistance.emplace_back(abs(from[i] - point[i]));
        }
    }

    double result = 0;
    for(auto & i : minDistance)
        result += i * i;
    return sqrt(result);
}

double RoutingEntry::calculateArea() const{
    vector<double> sideLength;
    sideLength.reserve(from.size());
    double result = 1;
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

bool RoutingEntry::intersects(const vector<int32_t> &searchFrom, const vector<int32_t> &searchTo) const{
    bool flag = true;
    pair<int32_t, int32_t> sortedPair;
    for (size_t i = 0; i < from.size(); ++i) {
        if(from[i] > to[i])
            sortedPair = make_pair(to[i], from[i]);
        else
            sortedPair = make_pair(from[i], to[i]);
        //Query ranges are sorted already (during input)
        if (!sortedIntervalsIntersect(make_pair(searchFrom[i], searchTo[i]), sortedPair)){
            flag = false;
            break;
        }
    }

    return flag;
}

bool RoutingEntry::sortedIntervalsIntersect(const pair<int32_t, int32_t> & i1, const pair<int32_t, int32_t> & i2) {
    bool flag = true;
    if(i1.second < i2.first)
        flag = false;
    if(i2.second < i1.first)
        flag = false;
    return flag;
}

double RoutingEntry::calculateAreaWithAnotherRoutingEntry(const shared_ptr<RoutingEntry> & anotherEntry) {
    RoutingEntry oneBigEntry;
    oneBigEntry.from.reserve(from.size());
    oneBigEntry.to.reserve(from.size());

    vector<int32_t> rangesForCertainDimension;
    for (size_t i = 0; i < from.size(); ++i) {
        rangesForCertainDimension.clear();
        rangesForCertainDimension = {from[i], anotherEntry->from[i], to[i]};
        if (anotherEntry->from.size() == anotherEntry->to.size())
            rangesForCertainDimension.push_back(anotherEntry->to[i]);
        size_t minIndex = min_element(rangesForCertainDimension.begin(), rangesForCertainDimension.end()) - rangesForCertainDimension.begin();
        size_t maxIndex = max_element(rangesForCertainDimension.begin(), rangesForCertainDimension.end()) - rangesForCertainDimension.begin();
        oneBigEntry.from.emplace_back(rangesForCertainDimension[minIndex]);
        oneBigEntry.to.emplace_back(rangesForCertainDimension[maxIndex]);
    }

    return oneBigEntry.calculateArea();
}


