#pragma once

#include "RoutingEntry.h"

///Flags and structures needed for propagating up when returning from recurse in tree insert method
struct RecurseInsertStruct {
    RoutingEntry createdEntrySurroundingNewNodeIfSplit;
    bool enlarged;
    bool split;
};