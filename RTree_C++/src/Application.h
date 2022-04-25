#pragma once

#include <set>
#include "CommandHandler.h"
#include "Action.h"
#include "DataRow.h"
#include "KnnSearchStruct.h"
#include "RTree.h"

///Wrapper class for whole application run (the main leading class)
class Application {
private:
    CommandHandler commandHandler;
    std::string dataFileName = "../res/data.txt"; //careful on file placement --> relative path depends on binary path (now in cmake-build-debug)
    RTree tree;

public:
    Application();

    ///Starts application and it's calculations and operations depending on user input/request
    void start();

    ///Makes sequence range search in data file (for comparing to R-tree queries)
    std::set<uint32_t> doTheRangeSearch(const std::vector<int32_t> & searchFrom, const std::vector<int32_t> & searchTo);

    std::set<KnnSearchStruct> doTheKnnSearch(const std::vector<int32_t> & queryPoint, const size_t & k);

    static bool containsPoint(const std::vector<int32_t> & row, const std::vector<int32_t> & searchFrom, const std::vector<int32_t> & searchTo);

    void generate();

    void rangeSearch(int action);

    void insert();

    void knnSearch();

    ///Prints result of the search on std::cout
    static void printResult(const std::set<uint32_t> & result);

    static void printResult(const std::set<KnnSearchStruct> & result);

    void writePointToDataFile(const DataRow & dataRow);

    size_t countLinesInDataFile();

    bool checkExistingFile();
};


