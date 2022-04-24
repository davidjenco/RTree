#include "CommandHandler.h"
#include "Action.h"

using namespace std;

CommandHandler::CommandHandler() {
    commands["generate"] = GENERATE;
    commands["search sequence range"] = SEQUENCE_RANGE_SEARCH;
    commands["search sequence knn"] = SEQUENCE_KNN_SEARCH;
    commands["search range"] = TREE_RANGE_SEARCH;
    commands["search knn"] = TREE_KNN_SEARCH;
    commands["insert"] = INSERT;
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
    cout << "\t\t" << " generate -d [DIMENSION]" << endl;
    cout << "\t\t" << " search --knn x1 x2 y1 y2 ..." << endl;
    cout << "\t\t" << " search --range x1 x2 y1 y2 ..." << endl;
    cout << "\t\t" << " search --sequence --range x1 x2 y1 y1 ..." << endl;
    cout << "\t\t" << " insert x y ..." << endl;
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
