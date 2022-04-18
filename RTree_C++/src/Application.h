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
    std::string dataFileName = "../data.txt"; //careful on file placement --> relative path depends on binary path (now in cmake-build-debug)

public:
    Application(int argc, char **argv);

    ///Starts application and it's calculations and operations depending on user input/request
    void start();

    ///Parses input and reacts on different actions
    void dealWithInput();

    ///Makes sequence range search in data file (for comparing to R-tree queries)
    void sequenceSearch();
};


