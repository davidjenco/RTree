#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>

///Class for handling commands
class CommandHandler {
private:
    std::map<std::string, int> commands;

    static void clearCin();

public:

    ///Creates new hardcoded map for our R-Tree application commands
    CommandHandler();

    ///Validates if command is correct and returns enum representation if yes
    int getAction(const std::string & cmd);

    ///Prints help table showing usage of application
    static void printHelpTable();

    ///Reads int value dimension from std::cin and clears rest of std::cin content. Returns 0 if conversion couldn't happen
    ///or non positive dimension was provided
    static int readDimension();

    ///Reads dimension * 2 int range values (form to, from to...) from std::cin and clears rest of std::cin content.
    ///Result is stored in search from/to in-out param. Returns false if conversion couldn't happen
    static bool readInputRanges(std::vector<int32_t> & searchFrom, std::vector<int32_t> & searchTo, const uint32_t & dimension);
};
