#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>

///Class for handling commands
class CommandHandler {
private:
    std::map<std::string, int> commands;

public:
    ///Creates new hardcoded map for our R-Tree application commands
    CommandHandler();

    int dealWithInput(std::string & input);

    ///Validates if command is correct and returns enum representation if yes
    int getAction(const std::string & cmd);

    ///Prints help table showing usage of application
    static void printHelpTable();

    int readDimension() const;

    int readInputRanges(std::vector<int32_t> & searchFrom, std::vector<int32_t> & searchTo, const uint32_t & dimension);
};
