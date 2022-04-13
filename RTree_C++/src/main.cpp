#include <iostream>
#include "Application.h"
#include "CommandHandler.h"

using namespace std;

int main(int argc, char * argv []) {
    Application treeApp(argc, argv);
    try {
        treeApp.start();
    } catch (invalid_argument & invalidArgument){
        CommandHandler::printHelpTable(argv[0]);
    }

    return 0;
}
