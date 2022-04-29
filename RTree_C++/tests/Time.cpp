#include <vector>
#include <iostream>
#include <sstream>
#include <set>
#include "../src/RTree.h"
#include "../src/Timer.h"
#include "../src/Application.h"
#include "../src/KnnSearchStruct.h"

#define DATA_PATH "../res/data.txt"

using namespace std;

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

int main(){
    Timer clock;
    RTree tree;
    tree.initStreamsExistingFile();
    tree.loadTree();

    set<KnnSearchStruct> knnResult;
    vector<int32_t> searchFrom = {13, 4443, 333, -1113};
    vector<int32_t> searchTo = {230, 4600, -33, -50};
    vector<int32_t> point = {230, 4600, -33, -50};

    clock.tick();
//    tree.rangeSearch(searchFrom, searchTo);
//    doTheRangeSearch(searchFrom, searchTo);
    tree.knnSearch(point, 20, knnResult);
//    doTheKnnSearch(point, 20, knnResult);
    clock.tock();

    cout << "Run time = " << clock.duration().count() << " ms\n";
    return 0;
}