#include <iostream>
#include "Job.h"

int main() {

    // Print to show that client is starting
    std::cout << "Starting client...\n";

    // Create job object
    Job job("jobA", 1, 4000, 2, "train_model.py");

    // Print details about job
    std::cout << "Created job:\n";
    std::cout << "ID: jobA\n";
    std::cout << "Payload: train_model.py\n";

    // Allow scheduler to assign job to worker
    std::cout << "\nSending job to scheduler...\n";

    return 0;

}