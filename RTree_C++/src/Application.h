#pragma once

#include "CommandHandler.h"
#include "Action.h"

///Wrapper class for whole application run (the main leading class)
class Application {
private:
    int dimension;
    int argc;
    char ** argv;
    CommandHandler commandHandler;
    std::string dataFileName = "../res/data.txt"; //careful on file placement --> relative path depends on binary path (now in cmake-build-debug)

public:
    Application(int argc, char **argv);

    ///Starts application and it's calculations and operations depending on user input/request
    void start();

    ///Parses input and reacts on different actions
    void dealWithInput();

    ///Makes sequence range search in data file (for comparing to R-tree queries)
    void sequenceSearch(const std::vector<int32_t> & searchFrom, const std::vector<int32_t> & searchTo);

    static bool containsPoint(const std::vector<int32_t> & row, const std::vector<int32_t> & searchFrom, const std::vector<int32_t> & searchTo);
};


