#include <iostream>
#include <Worker.h>

int main() {

    // Start process
    std::cout << "Starting worker process...\n\n";

    // Create worker
    Worker worker("workerA", 8, 2, 16000);

    // Print details about worker
    std::cout << "Worker created:\n";
    std::cout << "ID: workerA\n";
    std::cout << "CPUs: 8";
    std::cout << "GPUs: 2";
    std::cout << "Memory: 16000MB\n\n";

    std::cout << "Waiting for jobs...\n";

    return 0;

}