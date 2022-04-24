#include <fstream>
#include <sstream>
#include "Application.h"
#include "DataGenerator.h"
#include "RTree.h"

using namespace std;

Application::Application() {
    commandHandler = CommandHandler();
}

void Application::start() {
    string input;

    while (true){
        getline(cin, input);
        if (input == "q"){
            break;
        }

        switch (commandHandler.getAction(input)) {
            case GENERATE:{
                generate();
                break;
            }
            case SEQUENCE_RANGE_SEARCH:{
                rangeSearch(SEQUENCE_RANGE_SEARCH);
                break;
            }
            case SEQUENCE_KNN_SEARCH:{

                break;
            }
            case TREE_RANGE_SEARCH:{
                rangeSearch(TREE_RANGE_SEARCH);
                break;
            }
            case TREE_KNN_SEARCH:{

                break;
            }
            case INSERT:{

                break;
            }
            default:{
                continue;
            }
        }
    }
}

void Application::generate() {
    uint32_t dimension = CommandHandler::readDimension();
    if (!dimension)
        return;

    auto tree = RTree();
    tree.configInit(dimension);
    tree.initStreamsRecreateFile();
    tree.serializeInit();

    DataGenerator generator(dimension, dataFileName);
    generator.generate(tree);

    tree.saveConfig();
    tree.closeStreams();
    cout << "Done" << endl;
}

void Application::rangeSearch(int action) {
    auto tree = RTree();

    tree.initStreamsExistingFile();
    tree.loadTree();

    vector<int32_t> searchFrom;
    vector<int32_t> searchTo;

    if (!CommandHandler::readInputRanges(searchFrom, searchTo, tree.getConfig().dimension)){
        tree.closeStreams();
        return;
    }

    if (action == TREE_RANGE_SEARCH)
        printResult(tree.rangeSearch(searchFrom, searchTo));
    else
        printResult(doTheRangeSearch(searchFrom, searchTo));

    tree.closeStreams();
}

set<uint32_t> Application::doTheRangeSearch(const vector<int32_t> &searchFrom, const vector<int32_t> &searchTo) {
    ifstream dataInputFile (dataFileName);
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

        if (containsPoint(row, searchFrom, searchTo))
            results.insert(id);
//            printf("Searching %d in (%d-%d) and searching %d in (%d-%d)\n", row[0], searchFrom[0], searchTo[0], row[1], searchFrom[1], searchTo[1]);
    }

    dataInputFile.close();
    return results;
}

bool Application::containsPoint(const vector<int32_t> &row, const vector<int32_t> &searchFrom,
                                const vector<int32_t> &searchTo) {
    for (size_t j = 0; j < searchFrom.size(); ++j) {
        if (searchFrom[j] > row[j] || searchTo[j] < row[j]){
            return false;
        }
    }

    return true;
}

void Application::printResult(const set<uint32_t> &result) {
    for (auto & res : result) {
        cout << res << " ";
    }
    cout << endl;
}
