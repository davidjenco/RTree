#pragma once

#include "RoutingEntry.h"

///Flags and structures needed for propagating up when returning from recurse in tree insert method
struct RecurseInsertStruct {
    std::shared_ptr<RoutingEntry> createdEntrySurroundingNewNodeIfSplit;
    bool enlarged;
    bool split;
};