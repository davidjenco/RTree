#pragma once

#include <string>
#include "RTree.h"

///Class for generating random data used to build R-Tree
class DataGenerator {
private:
    int dimension;
    int min = 99;
    int max = 9999;
    int numberOfEntries = 3;
    std::string dataFileName;

public:
    DataGenerator(int dimension, std::string dataFileName);

    ///Generates hardcoded number of data depending on demanded dimension in hardcoded min max range and stores
    ///it into data file (current format is text file) and also inserts all of these values into the tree
    ///passed in constructor
    void generate(RTree &tree);

    ///Generate random integer in specific range
    int getRandomInt();
};


