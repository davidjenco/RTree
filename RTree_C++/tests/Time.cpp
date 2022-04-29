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
//    DataGenerator generator (1, DATA_PATH);
//    tree.initStreamsExistingFile();
//    tree.loadTree();

    vector<int32_t> searchFrom;
    vector<int32_t> searchTo;
//    set<KnnSearchStruct> knnResult;
//    vector<int32_t> point;

    for (int d = 1; d <= 17; ++d) {
        DataGenerator generator (d, DATA_PATH);
        generate(tree, generator, d);
        double totalTime = 0;
        for (int i = 0; i < 5000; ++i) {
            searchFrom.clear();
            searchTo.clear();
            for (int j = 0; j < d; ++j) {
                searchFrom.emplace_back(getRandomInt(3000, 6000));
                searchTo.emplace_back(getRandomInt(-6000, -3000));
            }
            //Timer clock;
            //clock.tick();
            auto start = chrono::high_resolution_clock::now();
            tree.rangeSearch(searchFrom, searchTo);
//    doTheRangeSearch(searchFrom, searchTo);
//    tree.knnSearch(point, 20, knnResult);
//    doTheKnnSearch(point, 20, knnResult);
            //clock.tock();
            auto end = chrono::high_resolution_clock::now();
            auto tmp = (double) chrono::duration_cast<chrono::nanoseconds>(end - start).count() * 1e-6;
            totalTime += tmp;
        }
        totalTime /= 5000;
        cout << setw(2) << d << ": "<< totalTime << " ms\n";
    }
    return 0;
}