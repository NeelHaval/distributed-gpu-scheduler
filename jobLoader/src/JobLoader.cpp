#include "JobLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

// Load jobs from file
std::vector<Job> loadJobsFromFile(const std::string& filename) {

    // Set up vector of jobs to store read jobs
    std::vector<Job> jobs;

    // Create input file stream
    std::ifstream file(filename);
    
    // If failed to open file
    if (!file.is_open()) {

        std::cerr << "Failed to open job file!\n";
        return jobs;

    }

    // Read each line of the loaded file
    std::string line;

    while (std::getline(file, line)) {

        // Ignore empty lines
        if (line.empty()) {

            continue;

        }

        // Otherwise extract, initialise and store job parameters
        std::stringstream ss(line);

        std::string jobID;
        std::string gpu;
        std::string memory;
        std::string cpu;
        std::string payload;
        std::string priority;
        std::string executionTime;

        std::getline(ss, jobID, '|');
        std::getline(ss, gpu, '|');
        std::getline(ss, memory, '|');
        std::getline(ss, cpu, '|');
        std::getline(ss, payload, '|');
        std::getline(ss, priority, '|');
        std::getline(ss, executionTime, '|');

        // Assign job priority
        JobPriority jobPriority;

        if (priority == "Low") {

            jobPriority = JobPriority::Low;

        }

        else if (priority == "Normal") {

            jobPriority = JobPriority::Normal;

        }

        else if (priority == "High") {

            jobPriority = JobPriority::High;

        }

        else if (priority == "Critical") {

            jobPriority = JobPriority::Critical;

        }

        else {

            std::cerr << "Invalid priority: " << priority << "\n";
            continue;

        }

        // Create job object
        Job job(jobID, std::stoi(gpu), std::stoi(memory), std::stoi(cpu), payload,
                std::stoi(executionTime), jobPriority);

        // Push this job into the vector
        jobs.push_back(job);

    }

    return jobs;

}