#include <iostream>
#include "Application.h"
#include "CommandHandler.h"

using namespace std;

int main() {
    Application treeApp;
    try {
        treeApp.start();
    } catch (runtime_error & runtimeError){ //thrown when working with files goes wrong
        cout << "Runtime error: " << runtimeError.what() << endl;
    }

    return 0;
}
