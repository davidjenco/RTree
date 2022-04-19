#pragma once

#include <vector>

struct DataRow{
    std::vector<int> ranges;
    size_t rowId;

    explicit DataRow(size_t rowId) : rowId(rowId) {}
};