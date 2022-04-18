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
    } catch (out_of_range & outOfRangeOfCommands){ //thrown when input is not found in command map in CommandHandler
        CommandHandler::printHelpTable(argv[0]);
    } catch (runtime_error & runtimeError){ //thrown when working with files goes wrong
        cout << "Runtime error: " << runtimeError.what() << endl;
    }

    return 0;
}
