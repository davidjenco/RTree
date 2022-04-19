#include <random>
#include <fstream>
#include <iostream>
#include <utility>
#include "DataGenerator.h"
#include "DataRow.h"

using namespace std;

DataGenerator::DataGenerator(int dimension, string dataFileName) : dimension(dimension),
                                                                                             dataFileName(std::move(dataFileName)) {}

void DataGenerator::generate(RTree &tree) {
    auto dataOutFile = ofstream(dataFileName);
    for (int i = 0; i < numberOfEntries; ++i) {
        DataRow data (i);
        dataOutFile << i << " ";
        cout << "Inserting data: " << i << " ";
        for (int j = 0; j < dimension; ++j) { //value for each dimension
            int random = this->getRandomInt();
            data.ranges.emplace_back(random);
            if (j == dimension - 1) {
                dataOutFile << random << endl;
                cout << random << endl;
                break;
            }
            dataOutFile << random << " ";
            cout << random << " ";
        }
        tree.insert(data);
    }
}

//https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
int DataGenerator::getRandomInt() {
    random_device rd;     // only used once to initialise (seed) engine
    mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> uni(min,max); // guaranteed unbiased

    return uni(rng);
}
