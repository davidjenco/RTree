#include <random>
#include <fstream>
#include "DataGenerator.h"

using namespace std;

DataGenerator::DataGenerator(int dimension, const string &dataFileName) : dimension(dimension),
                                                                          dataFileName(dataFileName) {}

void DataGenerator::generate() {
    auto dataOutFile = ofstream(dataFileName);
    for (int i = 0; i < numberOfEntries; ++i) {
        dataOutFile << i << " ";//TODO později bych asi přešel na binární soubor, zatím takto pro přehlednost
        for (int j = 0; j < dimension; ++j) { //value for each dimension
            if (j == dimension - 1) {
                dataOutFile << this->getRandomInt() << endl;
                break;
            }
            dataOutFile << this->getRandomInt() << " ";
        }
    }
}

//https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
int DataGenerator::getRandomInt() {
    random_device rd;     // only used once to initialise (seed) engine
    mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> uni(min,max); // guaranteed unbiased

    return uni(rng);
}
