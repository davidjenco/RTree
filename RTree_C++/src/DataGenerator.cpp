#include <random>
#include <fstream>
#include <utility>
#include "DataGenerator.h"

using namespace std;

DataGenerator::DataGenerator(int dimension, string dataFileName, const RTree &tree) : dimension(dimension),
                                                                                             dataFileName(std::move(dataFileName)),
                                                                                             tree(tree) {}

void DataGenerator::generate() {
    auto dataOutFile = ofstream(dataFileName);
    for (int i = 0; i < numberOfEntries; ++i) {
        vector<int> ranges;
        dataOutFile << i << " ";
        for (int j = 0; j < dimension; ++j) { //value for each dimension
            int random = this->getRandomInt();
            ranges.emplace_back(random);
            if (j == dimension - 1) {
                dataOutFile << random << endl;
                break;
            }
            dataOutFile << random << " ";
        }
        tree.insert(ranges);
    }
}

//https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
int DataGenerator::getRandomInt() {
    random_device rd;     // only used once to initialise (seed) engine
    mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> uni(min,max); // guaranteed unbiased

    return uni(rng);
}
