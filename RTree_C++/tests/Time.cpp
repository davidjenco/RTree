#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <utility>
#include <random>
#include "../src/RTree.h"
#include "../src/Timer.h"
#include "../src/Application.h"
#include "../src/KnnSearchStruct.h"
#include "../src/DataGenerator.h"

#define DATA_PATH "../res/data.txt"

using namespace std;

int getRandomInt(int from, int to) {
    random_device rd;     // only used once to initialise (seed) engine
    mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> uni(from, to); // guaranteed unbiased

    return uni(rng);
}



set<uint32_t> doTheRangeSearch(const vector<int32_t> &searchFrom, const vector<int32_t> &searchTo) {
    ifstream dataInputFile (DATA_PATH);
    set<uint32_t> results;
    vector<int32_t> row;
    row.reserve(searchFrom.size()); //TODO kdyžtak dát i do Application

    string line;
    while (getline(dataInputFile, line)){
        istringstream iss (line);
        uint32_t id;
        iss >> id;
        row.clear();
        int32_t value;
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

void doTheKnnSearch(const vector<int32_t> & queryPoint, const size_t & k, set<KnnSearchStruct> & result) {
    ifstream dataInputFile (DATA_PATH);
    vector<int32_t> row;
    row.reserve(queryPoint.size()); //TODO kdyžtak dát i do Application

    string line;
    while (getline(dataInputFile, line)){
        istringstream iss (line);
        uint32_t id;
        iss >> id;
        row.clear();
        int32_t value;
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

void generate(RTree & tree, DataGenerator & generator, int dimension){
    tree.closeStreams();

    tree = RTree();
    tree.configInit(dimension);
    tree.initStreamsRecreateFile();
    tree.serializeInit();

    generator.generate(tree);

    tree.saveConfig();
}


int main(){
    RTree tree;
    Timer clock;

    vector<int32_t> searchFrom;
    vector<int32_t> searchTo;
    set<KnnSearchStruct> result;
    int numberOfQueries = 1000; //TODO napsat do popisu
    double searchRangeTime;
    int dimension = 3;

    for (uint32_t entriesInNode = 10; entriesInNode <= 100; entriesInNode+=10) {
        DataGenerator generator (dimension, DATA_PATH);
        tree.config.minNodeEntries = entriesInNode;

        searchRangeTime = 0;
        generate(tree, generator, dimension);

        for (int i = 0; i < numberOfQueries; ++i) {
            searchFrom.clear();
            searchTo.clear();
            for (int j = 0; j < dimension; ++j) {
                searchFrom.emplace_back(getRandomInt(-7000, -3000));
                searchTo.emplace_back(getRandomInt(3000, 7000));
            }
            clock.tick();
            tree.rangeSearch(searchFrom, searchTo);
            clock.tock();
            searchRangeTime += (double)clock.duration().count() * 1e-6;

        }
        searchRangeTime /= numberOfQueries;
        cout << entriesInNode << ": " << searchRangeTime << endl;
    }


    return 0;
}

//auto start = chrono::high_resolution_clock::now();
//auto end = chrono::high_resolution_clock::now();
//auto tmp = (double) chrono::duration_cast<chrono::nanoseconds>(end - start).count() * 1e-6;