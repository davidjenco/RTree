#pragma once

///Class for generating random data used to build R-Tree
class DataGenerator {
private:
    int dimension;
    int min = 99;
    int max = 9999;
    int numberOfEntries = 100;

public:
    DataGenerator(int dimension);

    ///Generates hardcoded number of data depending on demanded dimension in hardcoded min max range and stores
    ///it into data file (current format is text file)
    void generate();

    ///Generate random integer in specific range
    int getRandomInt();
};


