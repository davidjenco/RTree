#include <fstream>
#include <sstream>
#include "Application.h"
#include "DataGenerator.h"
#include "RTree.h"

using namespace std;

Application::Application(int argc, char **argv) : argc(argc), argv(argv) {
    commandHandler = CommandHandler();
}

void Application::start() {

    dealWithInput();

    //TODO...
}

void Application::dealWithInput() {
    if (argc < 2)
        throw invalid_argument("Too few arguments");

    auto action = commandHandler.getAction(argv [0], argv[1]);
    switch (action) {
        case GENERATE: {
            if (argc != 4 || string("-d") != argv[2])
                throw invalid_argument("Invalid argument content or count");

            dimension = stoi(argv[3]);

            auto tree = RTree();
            tree.configInit(dimension);
            tree.initStreamsRecreateFile();
            tree.serializeInit();

            DataGenerator generator(dimension, dataFileName);
            generator.generate(tree);


            tree.saveConfig();
            tree.closeStreams();
            break;
        }
        case SEARCH: {
            auto tree = RTree();

            tree.initStreamsExistingFile();
            tree.loadTree();

            if (string("--sequence") == argv[2]){

                if (string("--range") != argv[3] || 2 * tree.getConfig().dimension != (argc - 4))
                    throw invalid_argument("Invalid argument - wrong number of ranges in query according to current tree");

                vector<int32_t> searchFrom;
                vector<int32_t> searchTo;
                for (uint32_t i = 0; i < 2 * tree.getConfig().dimension; i+=2) {
                    if (stoi(argv[i + 4]) <= stoi(argv[i + 5])){
                        searchFrom.push_back(stoi(argv[i + 4]));
                        searchTo.push_back(stoi(argv[i + 5]));
                    }else{
                        searchFrom.push_back(stoi(argv[i + 5]));
                        searchTo.push_back(stoi(argv[i + 4]));
                    }
                }

                sequenceSearch(searchFrom, searchTo);

            }
            else if (string("--range") == argv[2]){

                if (2 * tree.getConfig().dimension != (argc - 3))
                    throw invalid_argument("Invalid argument - wrong number of ranges in query according to current tree");

                vector<int32_t> searchFrom;
                vector<int32_t> searchTo;
                for (uint32_t i = 0; i < 2 * tree.getConfig().dimension; i+=2) {
                    if (stoi(argv[i + 3]) <= stoi(argv[i + 4])){
                        searchFrom.push_back(stoi(argv[i + 3]));
                        searchTo.push_back(stoi(argv[i + 4]));
                    }else{
                        searchFrom.push_back(stoi(argv[i + 4]));
                        searchTo.push_back(stoi(argv[i + 3]));
                    }
                }

                auto results = tree.rangeSearch(searchFrom, searchTo);

                for (auto & res : results) {
                    cout << res << " ";
                }
                cout << endl;
            }

            break;
        }
        case INSERT: {
            cout << "Insert" << endl; //dummy
            break;
        }
        default: break;
    }


}

void Application::sequenceSearch(const vector<int32_t> &searchFrom, const vector<int32_t> &searchTo) {
    ifstream dataInputFile (dataFileName); //TODO přiřadit jako stream do Application
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

    for (auto & res : results) {
        cout << res << " ";
    }
    cout << endl;

    dataInputFile.close();
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
