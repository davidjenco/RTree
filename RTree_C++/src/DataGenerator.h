#pragma once

#include <string>
#include "RTree.h"

///Class for generating random data used to build R-Tree
class DataGenerator {
private:
    uint32_t dimension;
    int min = -50000;
    int max = 50000;
    int numberOfEntries = 20000;
    std::string dataFileName;

public:
    DataGenerator(uint32_t dimension, std::string dataFileName);

    ///Generates hardcoded number of data depending on demanded dimension in hardcoded min max range and stores
    ///it into data file (current format is text file) and also inserts all of these values into the tree
    ///passed in constructor
    void generate(RTree &tree);

    ///Generate random integer in specific range
    int getRandomInt() const;
};


