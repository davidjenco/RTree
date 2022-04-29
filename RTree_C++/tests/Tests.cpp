#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <random>
#include <sstream>
#include "../src/InsertCandidate.h"
#include "../src/Node.h"
#include "../src/RTree.h"
#include "../src/CommandHandler.h"
#include "../src/Splitter.h"
#include "../src/DataGenerator.h"
#include "../src/Application.h"


#define DATA_PATH "../res/data.txt"

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
    vector<shared_ptr<RoutingEntry>> vec = {make_shared<RoutingEntry>(RoutingEntry(DataRow(89))),
                                            make_shared<RoutingEntry>(RoutingEntry(DataRow(2))),
                                            make_shared<RoutingEntry>(RoutingEntry(DataRow(9))),
                                            make_shared<RoutingEntry>(RoutingEntry(DataRow(4))),
                                            make_shared<RoutingEntry>(RoutingEntry(DataRow(39))),
                                            make_shared<RoutingEntry>(RoutingEntry(DataRow(19))),
                                            make_shared<RoutingEntry>(RoutingEntry(DataRow(6)))

    };
    vector<shared_ptr<RoutingEntry>> half2;

//    Splitter::randomSplit(vec, half2);

//    random_device rd;
//    mt19937 rng(rd());
//    shuffle(vec.begin(), vec.end(), rng);
//
//    size_t const half_size = vec.size() / 2;
//
//    half2.insert(half2.begin(), vec.begin() + half_size, vec.end());
//    vec.erase(vec.begin() + half_size, vec.end());

    cout << "vec: ";
    for( auto & i : vec )
        cout << i->childNodeId << " ";
    cout << endl;
    cout << "half2: ";
    for( auto & i : half2 )
        cout << i->childNodeId << " ";
    cout << endl;
}

//void testMBBfromNode(){
//
//    TreeConfig conf;
//    conf.dimension = 3;
//
//    RoutingEntry a;
//    RoutingEntry b;
//    RoutingEntry c;
//
//    a.from = {-10, 2, 3};
//    b.from = {2, 3, 1};
//    c.from = {3, 9, 2};
//    a.to   = {2, 10, 4};
//    b.to   = {3, 4, 2};
//    c.to   = {4, 2, 30};
//    vector<RoutingEntry> entries = {a, b, c};
//
//    Node n(1, true, entries);
//    RoutingEntry parent;
//
//    n.rewriteEntry(parent, conf);
//    assert( parent.from.size() == conf.dimension );
//    for (size_t i = 0; i < parent.from.size(); ++i) {
//        cout << i << ": <" << parent.from[i] << "; " << parent.to[i] << ">" << endl;
//    }
//    assert(parent.childNodeId == n.id);
//}

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
    tree.configInit(20);

    cout << tree.getConfig().maxNodeEntries << " in node and " << tree.getConfig().maxLeafNodeEntries << " in leaf" << endl;

    assert((tree.getConfig().nodeSizeInBytes - sizeof(tree.getRoot().id) - sizeof(tree.getRoot().isLeaf)) % tree.getConfig().maxNodeEntries == 0);
    assert((tree.getConfig().nodeSizeInBytes - sizeof(tree.getRoot().id) - sizeof(tree.getRoot().isLeaf)) % tree.getConfig().maxLeafNodeEntries == 0);

    cout << tree.getConfig().minPossibleNodeSize << " " << tree.getConfig().nodeSizeInBytes << endl;
}

void stoiTest(int argc, char *argv[]){
    try {
        cout << "zadal jsi: " << stoi(argv[1]) << endl;
    } catch(invalid_argument & e){
        cout << "Pozor co píšeš." << endl;
    }
}

void intervalIntersect(){
    assert(RoutingEntry::sortedIntervalsIntersect(make_pair(1, 2), make_pair(1, 3)) == true);
    assert(RoutingEntry::sortedIntervalsIntersect(make_pair(1, 2), make_pair(2, 3)) == true);
//    assert(RoutingEntry::sortedIntervalsIntersect(make_pair(2, 1), make_pair(3, 2)) == true);
    assert(RoutingEntry::sortedIntervalsIntersect(make_pair(1, 2), make_pair(3, 5)) == false);
//    assert(RoutingEntry::sortedIntervalsIntersect(make_pair(2, 1), make_pair(5, 3)) == false);
    assert(RoutingEntry::sortedIntervalsIntersect(make_pair(10, 16), make_pair(3, 5)) == false);
    assert(RoutingEntry::sortedIntervalsIntersect(make_pair(10, 16), make_pair(3, 11)) == true);
    assert(RoutingEntry::sortedIntervalsIntersect(make_pair(-23, -14), make_pair(-20, -11)) == true);
//    assert(RoutingEntry::sortedIntervalsIntersect(make_pair(16, 10), make_pair(11, 3)) == true);
}

void testInput(){
    CommandHandler cmd;
    int a = cmd.readDimension();
    int b = cmd.readDimension();

    cout << "Zadal jsi: " << a << " a potom " << b << endl;
}

void countLines(){
    std::fstream inFile(DATA_PATH, ios::app);
    cout << std::count(std::istreambuf_iterator<char>(inFile),
               std::istreambuf_iterator<char>(), '\n') << endl;
}

void doTheKnnSearch(const vector<int32_t> & queryPoint, const size_t & k, set<KnnSearchStruct> & result) {
    ifstream dataInputFile (DATA_PATH);

    string line;
    while (getline(dataInputFile, line)){
        istringstream iss (line);
        uint32_t id;
        iss >> id;
        int32_t value;
        vector<int32_t> row;
        while (iss >> value){
            row.emplace_back(value);
        }

        double distance = RoutingEntry (DataRow(row, id)).calculateDistance(queryPoint);
        if (result.size() < k)
            result.insert(KnnSearchStruct(id, distance));
        else if (distance < result.rbegin()->distance){
            result.insert(KnnSearchStruct(id, distance));
            result.erase(--result.end());
        }
    }

    if (dataInputFile.fail() && !dataInputFile.eof()){
        dataInputFile.close();
        throw runtime_error("Error while reading the data file");
    }
    dataInputFile.close();
}

set<uint32_t> doTheRangeSearch(const vector<int32_t> &searchFrom, const vector<int32_t> &searchTo) {
    ifstream dataInputFile (DATA_PATH);
    set<uint32_t> results;

    string line;
    while (getline(dataInputFile, line)){
        istringstream iss (line);
        uint32_t id;
        iss >> id;
        int32_t value;
        vector<int32_t> row;
        while (iss >> value){
            row.emplace_back(value);
        }

        if (Application::containsPoint(row, searchFrom, searchTo))
            results.insert(id);
//            printf("Searching %d in (%d-%d) and searching %d in (%d-%d)\n", row[0], searchFrom[0], searchTo[0], row[1], searchFrom[1], searchTo[1]);
    }

    if (dataInputFile.fail() && !dataInputFile.eof()){
        dataInputFile.close();
        throw runtime_error("Error while reading the data file");
    }
    dataInputFile.close();
    return results;
}

void generate(RTree & tree, DataGenerator & generator, int dimension){
    tree.closeStreams();

    tree = RTree();
    tree.configInit(dimension);
    tree.initStreamsRecreateFile();
    tree.serializeInit();


    generator.generate(tree);

    tree.saveConfig();
}

bool testOneKnn(RTree & tree, DataGenerator & generator, int dimension){
    vector<int32_t> point;
    point.reserve(dimension);

    for (int i = 0; i < dimension; ++i) {
        point.emplace_back(generator.getRandomInt());
    }

    set<KnnSearchStruct> result1;
    set<KnnSearchStruct> result2;
    tree.knnSearch(point, 20, result1);
    doTheKnnSearch(point, 20, result2);

    double max = result1.rbegin()->distance;
    result1.erase(result1.lower_bound(KnnSearchStruct(0, max)), result1.end());
    result2.erase(result2.lower_bound(KnnSearchStruct(0, max)), result2.end());

    return result1 == result2;
}

void testOneSearch(RTree & tree, int dimension){
    vector<int32_t> from, to;
    DataGenerator generator(dimension, DATA_PATH);
    generate(tree, generator, dimension);

    for (int j = 0; j < 10; ++j) {
        from.clear();
        to.clear();
        for (int i = 0; i < dimension; ++i) {
            from.emplace_back(generator.getRandomInt());
            to.emplace_back(generator.getRandomInt());
        }
        assert(tree.rangeSearch(from, to) == doTheRangeSearch(from, to));
        assert(testOneKnn(tree, generator, dimension));
    }
}

void searchTest(){
    RTree tree;

    //iterates through different dimensions
    for (int dimension = 1; dimension <= 10; ++dimension) {
        testOneSearch(tree, dimension);
    }
}

int main (int argc, char *argv[]){
//    testCandidate();
//    testMBBfromNode();
//    rewriteInFile();
//    readWriteNodes();
//    testCalculations();
//    stoiTest(argc, argv);

//    intervalIntersect();
//    testInput();
//    countLines();
//    testCalculations();
//    testDistributeEntriesOnHalves();


    searchTest();

    return 0;
}
