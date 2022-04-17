#include "CommandHandler.h"
#include "Action.h"

using namespace std;

CommandHandler::CommandHandler() {
    commands["generate"] = GENERATE;
    commands["search"] = SEARCH;
    commands["insert"] = INSERT;
}

int CommandHandler::getAction(const string & binary, const string & cmd) {
    return commands.at(cmd);
}

void CommandHandler::printHelpTable(const string &binary) {
    cout << "Usage help:" << endl;
    cout << "\t\t" << binary << " generate -d [DIMENSION]" << endl;
    cout << "\t\t" << binary << " search --knn x1 x2 y1 y2 ..." << endl;
    cout << "\t\t" << binary << " search --range x1 x2 y1 y2 ..." << endl;
    cout << "\t\t" << binary << " search --sequence --range x1 x2 y1 y1 ..." << endl;
    cout << "\t\t" << binary << " insert x y ..." << endl;
}
