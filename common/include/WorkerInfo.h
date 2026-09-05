#pragma once
#include <string>
#include "WorkerState.h"

struct WorkerInfo {

    std::string workerID;

    int availableCPUs;
    int totalCPUs;

    int availableGPUs;
    int totalGPUs;

    size_t availableMem;
    size_t totalMem;

    WorkerState state;
    
    std::string currentJobID;

};