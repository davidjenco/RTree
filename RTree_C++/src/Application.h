#pragma once

#include <set>
#include "CommandHandler.h"
#include "Action.h"
#include "DataRow.h"

///Wrapper class for whole application run (the main leading class)
class Application {
private:
    CommandHandler commandHandler;
    std::string dataFileName = "../res/data.txt"; //careful on file placement --> relative path depends on binary path (now in cmake-build-debug)

public:
    Application();

    ///Starts application and it's calculations and operations depending on user input/request
    void start();

    ///Makes sequence range search in data file (for comparing to R-tree queries)
    std::set<uint32_t> doTheRangeSearch(const std::vector<int32_t> & searchFrom, const std::vector<int32_t> & searchTo);

    static bool containsPoint(const std::vector<int32_t> & row, const std::vector<int32_t> & searchFrom, const std::vector<int32_t> & searchTo);

    void generate();

    void rangeSearch(int action);

    void insert();

    ///Prints result of the search on std::cout
    static void printResult(const std::set<uint32_t> & result);

    void writePointToDataFile(const DataRow & dataRow);

    size_t countLinesInDataFile();
};


