#pragma once
#include "Job.h"
#include <string>
#include <vector>

// Load jobs from file
std::vector<Job> loadJobsFromFile(const std::string& filename);