#include <fstream>
#include <sstream>
#include <filesystem>
#include "Application.h"
#include "DataGenerator.h"
#include "RTree.h"

using namespace std;
using namespace std::filesystem;

Application::Application() {
    commandHandler = CommandHandler();
    tree = RTree();
}

void Application::start() {
    tree.initStreamsExistingFile();
    string input;

    while (true){
        getline(cin, input);
        if (input.empty())
            continue;
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
                knnSearch(SEQUENCE_KNN_SEARCH);
                break;
            }
            case TREE_RANGE_SEARCH:{
                rangeSearch(TREE_RANGE_SEARCH);
                break;
            }
            case TREE_KNN_SEARCH:{
                knnSearch(TREE_KNN_SEARCH);
                break;
            }
            case INSERT:{
                insert();
                break;
            }
            case HELP:{
                commandHandler.printHelpTable();
                break;
            }
            case QUIT:{
                tree.closeStreams();
                return;
            }
            default:{
                continue;
            }
        }
    }
}

void Application::generate() {
    tree.closeStreams();

    uint32_t dimension = CommandHandler::readDimension();
    if (!dimension)
        return;

    tree = RTree();
    tree.configInit(dimension);
    tree.initStreamsRecreateFile();
    tree.serializeInit();

    DataGenerator generator(dimension, dataFileName);
    generator.generate(tree);

    tree.saveConfig();
    cout << "Done" << endl;
}

void Application::rangeSearch(int action) {
    if (!checkExistingFile())
        return;
    tree.loadTree();

    vector<int32_t> searchFrom;
    vector<int32_t> searchTo;

    if (!CommandHandler::readInputRanges(searchFrom, searchTo, tree.getConfig().dimension)){
        return;
    }

    if (action == TREE_RANGE_SEARCH)
        printResult(tree.rangeSearch(searchFrom, searchTo));
    else
        printResult(doTheRangeSearch(searchFrom, searchTo));
}

void Application::insert() {
    if (!checkExistingFile())
        return;
    tree.loadTree();

    DataRow dataRow(countLinesInDataFile());
    if (!CommandHandler::readInputPoint(dataRow.ranges, tree.getConfig().dimension)){
        return;
    }

    tree.insert(dataRow);
    writePointToDataFile(dataRow);
    tree.saveConfig();
    cout << "Done" << endl;
    tree.clearCache();
}

void Application::knnSearch(int action) {
    if (!checkExistingFile())
        return;
    tree.loadTree();

    vector<int32_t> queryPoint;
    if (!CommandHandler::readInputPoint(queryPoint, tree.getConfig().dimension)){
        tree.closeStreams();
        return;
    }
    int k = CommandHandler::readNumberOfNeighbours();
    if (!k)
        return;

    set<KnnSearchStruct> result;
    if (action == SEQUENCE_KNN_SEARCH){
        doTheKnnSearch(queryPoint, k, result);
        printResult(result);
    }
    else{
        tree.knnSearch(queryPoint, k, result);
        printResult(result);
    }
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

    if (dataInputFile.fail() && !dataInputFile.eof()){
        dataInputFile.close();
        throw runtime_error("Error while reading the data file");
    }
    dataInputFile.close();
    return results;
}

void Application::doTheKnnSearch(const vector<int32_t> & queryPoint, const size_t & k, set<KnnSearchStruct> & result) {
    ifstream dataInputFile (dataFileName);

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

void Application::printResult(const set<KnnSearchStruct> &result) {
    for (auto & res : result) {
        cout << res.node->id << " ";
    }
    cout << endl;
}

void Application::writePointToDataFile(const DataRow & dataRow) {
    ofstream dataOutputFile (dataFileName, ios::app);

    dataOutputFile << dataRow.rowId << " ";
    for (size_t i = 0; i < dataRow.ranges.size(); ++i) {
        if (i == dataRow.ranges.size() - 1)
            dataOutputFile << dataRow.ranges[i] << endl;
        else
            dataOutputFile << dataRow.ranges[i] << " ";
    }
}

size_t Application::countLinesInDataFile() {
    std::ifstream inFile(dataFileName);
    size_t lines = std::count(std::istreambuf_iterator<char>(inFile),std::istreambuf_iterator<char>(), '\n');
    inFile.close();
    return lines;
}

bool Application::checkExistingFile() {
    bool flag = true;
    if (!exists("../res/tree")){
        cout << "You have to generate tree first" << endl;
        commandHandler.printHelpTable();
        flag = false;
    }
    return flag;
}
