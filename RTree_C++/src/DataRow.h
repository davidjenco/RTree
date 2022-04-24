#pragma once

#include <utility>
#include <vector>

///Represents one row of data - ranges stands for values for all dimensions and rowId is line number of such entry
///in data file
struct DataRow{
    std::vector<int> ranges;
    size_t rowId;

    explicit DataRow(size_t rowId) : rowId(rowId) {}

    DataRow(std::vector<int> ranges, size_t rowId) : ranges(std::move(ranges)), rowId(rowId) {}
};