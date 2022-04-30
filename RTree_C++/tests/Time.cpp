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
    Timer clock;
//    DataGenerator generator (1, DATA_PATH);
//    tree.initStreamsExistingFile();
//    tree.loadTree();

    vector<int32_t> searchFrom;
    vector<int32_t> searchTo;
    set<KnnSearchStruct> knnResult;
    int numberOfQueries = 100; //TODO napsat do popisu
    double searchRangeTime, sequenceRangeTime, searchKnnTime, sequenceKnnTime;

    for (int d = 14; d <= 14; ++d) {
        DataGenerator generator (d, DATA_PATH);
        generate(tree, generator, d);
        searchRangeTime = sequenceRangeTime = searchKnnTime = sequenceKnnTime = 0;
        for (int i = 0; i < numberOfQueries; ++i) {
            searchFrom.clear();
            searchTo.clear();
            for (int j = 0; j < d; ++j) {
                searchFrom.emplace_back(getRandomInt(15000, 30000)); //TODO napsat do popisu
                searchTo.emplace_back(getRandomInt(-30000, -15000));
            }
            clock.tick();
            tree.rangeSearch(searchFrom, searchTo);
            clock.tock();
            searchRangeTime += (double)clock.duration().count() * 1e-6;

            clock.tick();
            doTheRangeSearch(searchFrom, searchTo);
            clock.tock();
            sequenceRangeTime += (double)clock.duration().count() * 1e-6;

            clock.tick();
            tree.knnSearch(searchFrom, 20, knnResult);
            clock.tock();
            searchKnnTime += (double)clock.duration().count() * 1e-6;

            clock.tick();
            doTheKnnSearch(searchFrom, 20, knnResult);
            clock.tock();
            sequenceKnnTime += (double)clock.duration().count() * 1e-6;
        }
        searchRangeTime /= numberOfQueries;
        sequenceRangeTime /= numberOfQueries;
        searchKnnTime /= numberOfQueries;
        sequenceKnnTime /= numberOfQueries;
        printf("%2d: %g %g | %g %g\n", d, searchRangeTime, sequenceRangeTime, searchKnnTime, sequenceKnnTime);
        //cout << setw(2) << d << ": " << searchRangeTime << "  " << " ms\n";
    }
    return 0;
}

//auto start = chrono::high_resolution_clock::now();
//auto end = chrono::high_resolution_clock::now();
//auto tmp = (double) chrono::duration_cast<chrono::nanoseconds>(end - start).count() * 1e-6;