#include <iostream>
#include <thread>
#include "../include/StackTracer.h"
#include <vector>
#include <unistd.h>

void division_by_zero(int thread_id) {
    if (thread_id == 2) {
        int x = 0;
        int y = 42 / x;  // Division by zero, causes crash or undefined behavior
        std::cout << "Result: " << y << std::endl;
    }
    std::cout << "Thread " << thread_id << " completed\n";
}

int main() {

    StackTracer* stack_tracer = StackTracer::getInstance();
    stack_tracer->setupStackTracer(); // Ensure correct arguments   

    const int num_threads = 5;  // Number of threads
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.push_back(std::thread(division_by_zero, i));
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
