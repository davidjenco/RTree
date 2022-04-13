#include <fstream>
#include "Application.h"
#include "DataGenerator.h"

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
            DataGenerator generator(dimension);
            generator.generate();
            break;
        }
        case SEARCH: {
            if (argc != 10 || string("--sequence") != argv[2] || string("--range") != argv[3])
                throw invalid_argument("Invalid argument content or count");
            vector<pair<int, int>> ranges;
            ranges.emplace_back(stoi(argv[4]), stoi(argv[5]));
            ranges.emplace_back(stoi(argv[6]), stoi(argv[7]));
            ranges.emplace_back(stoi(argv[8]), stoi(argv[9])); //this is not parametrized and works only for 3 dimensional data

            vector<int> foundIds;

            ifstream ifs("../data.txt");
            while (!ifs.eof()){
                int id, dim1, dim2, dim3;
                ifs >> id >> dim1 >> dim2 >> dim3;
                if (dim1 >= ranges[0].first && dim1 <= ranges[0].second &&
                    dim2 >= ranges[1].first && dim2 <= ranges[1].second &&
                    dim3 >= ranges[2].first && dim3 <= ranges[2].second){
                    foundIds.emplace_back(id);
                }
            }

            for (const auto & id : foundIds) {
                cout << "Found " << id << endl;
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
