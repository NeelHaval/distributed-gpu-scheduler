#pragma once
#include <string>
#include "WorkerState.h"
#include <chrono>

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

    // Heartbeat book keeping
    std::chrono::steady_clock::time_point lastHB;
    int missedHBs;

};