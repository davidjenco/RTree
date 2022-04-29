#include <random>
#include "Splitter.h"

using namespace std;

void Splitter::randomSplit(Node & fullNode, Node & node1, Node & node2) {
    random_device rd;
    mt19937 rng(rd());
    shuffle(fullNode.entries.begin(), fullNode.entries.end(), rng);

    size_t const half_size = fullNode.entries.size() / 2;
//    vector<shared_ptr<RoutingEntry>> half1 (fullNode.entries.begin(), fullNode.entries.begin() + half_size);
//    vector<shared_ptr<RoutingEntry>> half2 (fullNode.entries.begin() + half_size, fullNode.entries.end());
//    fullNode.entries = half1;
//    newNodeEntries = half2;

    node1.entries.insert(node1.entries.begin(), fullNode.entries.begin(), fullNode.entries.begin() + half_size);
    node2.entries.insert(node2.entries.begin(), fullNode.entries.begin() + half_size, fullNode.entries.end());
}

void Splitter::quadraticSplit(Node &fullNode, Node &node1, Node &node2, const TreeConfig & config) {
    size_t halfNumOfNodes = fullNode.entries.size() / 2;
    pair<size_t, size_t> seeds = quadraticPickSeeds(fullNode);

    node1.entries.push_back(fullNode.entries[seeds.first]);
    node2.entries.push_back(fullNode.entries[seeds.second]);
    fullNode.entries.erase(fullNode.entries.begin() + seeds.second);
    fullNode.entries.erase(fullNode.entries.begin() + seeds.first);

    quadraticDistribute(fullNode, node1, node2, halfNumOfNodes, config);
}

pair<size_t, size_t> Splitter::quadraticPickSeeds(Node &fullNode) {
    double maxArea = 0;
    pair<size_t, size_t> seeds;
    for (size_t i = 0; i < fullNode.entries.size() - 1; ++i) {
        for (size_t j = i + 1; j < fullNode.entries.size(); ++j) {
            double inBetweenArea;
            if (fullNode.isLeaf)
                inBetweenArea = fullNode.entries[i]->calculateDistance(fullNode.entries[j]->from);
            else
                inBetweenArea = fullNode.entries[i]->calculateAreaWithAnotherRoutingEntry(fullNode.entries[j])
                    - fullNode.entries[i]->calculateArea() - fullNode.entries[j]->calculateArea();
            if (inBetweenArea > maxArea){
                maxArea = inBetweenArea;
                seeds = make_pair(i, j);
            }
        }
    }

    return seeds;
}

size_t Splitter::pickNext(Node &fullNode, shared_ptr<RoutingEntry> & surroundingNode1,
                          shared_ptr<RoutingEntry> & surroundingNode2, const double areaNode1, const double areaNode2, const TreeConfig & config) {
    double maxAreaDifference = -1;
    size_t nextIndex;

    for (size_t i = 0; i < fullNode.entries.size(); ++i) {
        double node1IncreaseByEntry = surroundingNode1->calculateAreaWithAnotherRoutingEntry(fullNode.entries[i]) - areaNode1;
        double node2IncreaseByEntry = surroundingNode2->calculateAreaWithAnotherRoutingEntry(fullNode.entries[i]) - areaNode2;
        if (double res = abs(node1IncreaseByEntry - node2IncreaseByEntry); res > maxAreaDifference){
            maxAreaDifference = res;
            nextIndex = i;
        }
    }
    return nextIndex;
}

void Splitter::quadraticDistribute(Node &fullNode, Node &node1, Node &node2, const size_t & halfNumOfNodes, const TreeConfig & config) {
    shared_ptr<RoutingEntry> surroundingEntryNode1 = make_shared<RoutingEntry>();
    shared_ptr<RoutingEntry> surroundingEntryNode2 = make_shared<RoutingEntry>();

    while (!fullNode.entries.empty() && node1.entries.size() != halfNumOfNodes && node2.entries.size() != halfNumOfNodes){
        node1.rewriteEntry(surroundingEntryNode1, config);
        double areaNode1 = surroundingEntryNode1->calculateArea();
        node2.rewriteEntry(surroundingEntryNode2, config);
        double areaNode2 = surroundingEntryNode2->calculateArea();

        size_t nextIndex = pickNext(fullNode, surroundingEntryNode1, surroundingEntryNode2, areaNode1, areaNode2, config);
        auto nextEntryToInsert = fullNode.entries[nextIndex];
        double increasedAreaNode1 = surroundingEntryNode1->calculateAreaWithAnotherRoutingEntry(fullNode.entries[nextIndex]) - areaNode1;
        double increasedAreaNode2 = surroundingEntryNode2->calculateAreaWithAnotherRoutingEntry(fullNode.entries[nextIndex]) - areaNode2;
        if (increasedAreaNode1 < increasedAreaNode2)
            node1.entries.emplace_back(nextEntryToInsert);
        else if (increasedAreaNode2 < increasedAreaNode1)
            node2.entries.emplace_back(nextEntryToInsert);
        else{
            shared_ptr<RoutingEntry> tmpMBB = make_shared<RoutingEntry>();
            node1.rewriteEntry(tmpMBB, config);
            double areaNode1 = tmpMBB->calculateArea();
            node2.rewriteEntry(tmpMBB, config);
            double areaNode2 = tmpMBB->calculateArea();

            if (areaNode1 < areaNode2)
                node1.entries.emplace_back(nextEntryToInsert);
            else if (areaNode2 < areaNode1)
                node2.entries.emplace_back(nextEntryToInsert);
            else if (node1.entries.size() < node2.entries.size())
                node1.entries.emplace_back(nextEntryToInsert);
            else
                node2.entries.emplace_back(nextEntryToInsert);
        }
        fullNode.entries.erase(fullNode.entries.begin() + nextIndex);
    }

    if (node1.entries.size() == halfNumOfNodes)
        node2.entries.insert(node2.entries.begin(), fullNode.entries.begin(), fullNode.entries.end());
    else
        node1.entries.insert(node1.entries.begin(), fullNode.entries.begin(), fullNode.entries.end());
}
