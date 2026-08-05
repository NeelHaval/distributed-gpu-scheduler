#pragma once
#include <string>
#include "WorkerState.h"

struct WorkerInfo {

    std::string workerID;
    int availableCPUs;
    int availableGPUs;
    size_t availableMem;
    WorkerState state;
    std::string currentJobID;

};