#include "CommandHandler.h"
#include "Action.h"

using namespace std;

CommandHandler::CommandHandler() {
    commands["generate"] = GENERATE;
    commands["search range -s"] = SEQUENCE_RANGE_SEARCH;
    commands["search knn -s"] = SEQUENCE_KNN_SEARCH;
    commands["search range"] = TREE_RANGE_SEARCH;
    commands["search knn"] = TREE_KNN_SEARCH;
    commands["insert"] = INSERT;
    commands["help"] = HELP;
    commands["quit"] = QUIT;
}

int CommandHandler::getAction(const string & cmd) {
    if (!commands.contains(cmd)){
        printHelpTable();
        return -1;
    }
    return commands.at(cmd);
}

void CommandHandler::printHelpTable() {
    cout << "Usage help:" << endl;
    for (auto & cmd : commands) {
        cout << "\t\t" << cmd.first << endl;
    }
}

int CommandHandler::readDimension() {
    int dim;

    cout << "Enter dimension: " << endl;
    if(!(cin >> dim) || dim < 1) {
        cout << "Dimension must be positive integer" << endl;
        clearCin();
        return 0;
    }
    clearCin();

    return dim;
}



bool CommandHandler::readInputRanges(vector<int32_t> &searchFrom, vector<int32_t> &searchTo, const uint32_t &dimension) {
    cout << "Enter ranges for dimension " << dimension << endl;
    for (uint32_t i = 0; i < dimension; ++i) {
        int32_t from, to;

        if(!(cin >> from >> to)){
            clearCin();
            cout << "Wrong range entered" << endl;
            return false;
        }

        if (from <= to){
            searchFrom.push_back(from);
            searchTo.push_back(to);
        }else{
            searchFrom.push_back(to);
            searchTo.push_back(from);
        }
    }

    clearCin();

    return true;
}

void CommandHandler::clearCin() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool CommandHandler::readInputPoint(vector<int32_t> &point, const uint32_t &dimension) {
    cout << "Enter point ranges for dimension " << dimension << endl;
    for (uint32_t i = 0; i < dimension; ++i) {
        int32_t range;

        if(!(cin >> range)){
            clearCin();
            cout << "Wrong point range entered" << endl;
            return false;
        }
        point.push_back(range);
    }
    clearCin();
    return true;
}

int CommandHandler::readNumberOfNeighbours() {
    int count;

    cout << "Enter number of neighbours: " << endl;
    if(!(cin >> count) || count < 1) {
        cout << "Number of neighbours must be positive integer" << endl;
        clearCin();
        return 0;
    }
    clearCin();

    return count;
}
