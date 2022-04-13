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

    ///Validates if command is correct and returns enum representation if yes
    int getAction(const std::string & binary, const std::string & cmd);

    ///Prints help table showing usage of application
    static void printHelpTable(const std::string & binary);
};


