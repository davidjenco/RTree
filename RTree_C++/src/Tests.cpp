#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <random>
#include "InsertCandidate.h"
#include "Node.h"
#include "RTree.h"

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

    a.from = {-10, 2, 3};
    b.from = {2, 3, 1};
    c.from = {3, 9, 2};
    a.to   = {2, 10, 4};
    b.to   = {3, 4, 2};
    c.to   = {4, 2, 30};
    vector<RoutingEntry> entries = {a, b, c};

    Node n(1, true, entries);
    RoutingEntry parent;

    n.createEntry(parent, conf);
    assert( parent.from.size() == conf.dimension );
    for (size_t i = 0; i < parent.from.size(); ++i) {
        cout << i << ": <" << parent.from[i] << "; " << parent.to[i] << ">" << endl;
    }
    assert(parent.childNodeId == n.id);
}

void swapInVec(){
    vector<int> vec = {1, 2, 3};
    vector<int> vec2 = {3, 4, 5};

    swap(vec[1], vec2[1]);
    for(auto & i : vec)
        cout << i << " ";
    cout << endl;
    for( auto & i : vec2 )
        cout << i << " ";
    cout << endl;
}

void rewriteInFile(){
    ofstream ofs ("../testtt");
    int justTestInt = 2048;
    int anotherTestInt = 1;

    ofs.seekp(0);
    ofs.write((char *) & justTestInt, sizeof(justTestInt));
    ofs.seekp(0);
    ofs.write((char *) & anotherTestInt, sizeof(anotherTestInt));
    ofs.seekp(ofs.tellp());
//    ofs.seekp(4);
//    ofs.seekp(ofstream::end);
    ofs.write((char *) & justTestInt, sizeof(justTestInt));

    ofs.close();
}

size_t getFileSize(TreeConfig & config){
    ifstream treeFile(config.treeFileName, ios::binary);
    treeFile.seekg(ios::end);
    size_t fileSize = treeFile.tellg();
    treeFile.seekg(ios::beg);
    return fileSize;
}

void readWriteNodes(){

    auto tree = RTree();
    tree.configInit(2);
    tree.initStreamsRecreateFile();
    tree.serializeInit();

//    TreeConfig config = tree.getConfig();
//
//    RoutingEntry simple;
//    simple.childNodeId = 10;
//    simple.from = {100, 120, 130};
//
//    RoutingEntry simpleLeaf;
//    simpleLeaf.childNodeId = 11;
//    simple.from = {100, 120, 130};
//    simple.to = {-10, -120, -130};
//
//    Node leaf;
//    leaf.id = 2;
//    leaf.isLeaf = true;
//    leaf.entries.assign(10, simpleLeaf);

    tree.closeStreams();
}

void testCalculations(){
    RTree tree;
    tree.configInit(4);

    cout << tree.getConfig().maxNodeEntries << " in node and " << tree.getConfig().maxLeafNodeEntries << " in leaf" << endl;

    assert((tree.getConfig().nodeSizeInBytes - sizeof(tree.getRoot().id) - sizeof(tree.getRoot().isLeaf)) % tree.getConfig().maxNodeEntries == 0);
    assert((tree.getConfig().nodeSizeInBytes - sizeof(tree.getRoot().id) - sizeof(tree.getRoot().isLeaf)) % tree.getConfig().maxLeafNodeEntries == 0);

    cout << tree.getConfig().minPossibleNodeSize << endl;
}

int main (){
//    testCandidate();
//    testMBBfromNode();
//    rewriteInFile();
//    readWriteNodes();

    testCalculations();
}
