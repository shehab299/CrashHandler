#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>
#include "../include/StackTracer.h"

void infinite_recursion(int thread_id) {
    if (thread_id == 3) {
        // Thread 3 causes a stack overflow by recursive function call
        infinite_recursion(thread_id); // Recursive call without base case
    }
    std::cout << "Thread " << thread_id << " completed\n";
}

int main() {

    StackTracer* instance = StackTracer::getInstance();
    instance->setupStackTracer();

    const int num_threads = 5;  // Number of threads
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.push_back(std::thread(infinite_recursion, i));
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
