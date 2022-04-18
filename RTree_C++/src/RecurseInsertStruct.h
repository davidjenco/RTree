#pragma once

#include "RoutingEntry.h"

struct RecurseInsertStruct {
    RoutingEntry createdEntrySurroundingNewNodeIfSplit;
    bool enlarged;
    bool split;
};