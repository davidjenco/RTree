#include <vector>
#include <iostream>
#include <cassert>
#include <random>
#include "InsertCandidate.h"
#include "Node.h"

using namespace std;

void testCandidate(){
    InsertCandidate a(10, 10);
    InsertCandidate b(120, 100);
    InsertCandidate c(0.1, 1000);
    InsertCandidate d(0.1, 100);

    vector<InsertCandidate> vec = {a, b, c, d};

    assert((min_element(vec.begin(), vec.end()) - vec.begin()) == 3);
}

void testDistributeEntriesOnHalves(){
    vector<int> vec = {89, 15, 51, 27, 98};

    random_device rd;
    mt19937 rng(rd());
    shuffle(vec.begin(), vec.end(), rng);

    size_t const half_size = vec.size() / 2;
    vector<int> half1(vec.begin(), vec.begin() + half_size);
    vector<int> half2(vec.begin() + half_size, vec.end());
    cout << "half1: ";
    for( auto & i : half1 )
        cout << i << " ";
    cout << endl;
    cout << "half2: ";
    for( auto & i : half2 )
        cout << i << " ";
    cout << endl;
}

void testMBBfromNode(){

    TreeConfig conf;
    conf.dimension = 3;

    RoutingEntry a;
    RoutingEntry b;
    RoutingEntry c;

    a.from = {1, 2, 3};
    a.to = {2, 3, 4};
    b.from = {1, 2, 3};
    b.to = {2, 3, 4};
    c.from = {1, 2, 3};
    c.to = {2, 3, 4};
    vector<RoutingEntry> entries = {a, b, c};
    Node n(1, false, entries);

    RoutingEntry parent;
    parent.createFromNode(n, conf);
}

int main (){
    testCandidate();
    testDistributeEntriesOnHalves();
}
