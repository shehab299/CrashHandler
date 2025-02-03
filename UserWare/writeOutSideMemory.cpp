#include <iostream>
#include <thread>
#include <vector>
#include "../include/StackTracer.h"
#include <unistd.h>

void memory_corruption(int thread_id) {
    if (thread_id == 4) {
        int* arr = new int[5];
        arr[10] = 42;  // Write outside allocated memory, causing memory corruption
        delete[] arr;
    }
    std::cout << "Thread " << thread_id << " completed\n";
}

int main() {
    const int num_threads = 5;  // Number of threads
    std::vector<std::thread> threads;

    StackTracer* instance = StackTracer::getInstance();
    instance->setupStackTracer();

    for (int i = 0; i < num_threads; ++i) {
        threads.push_back(std::thread(memory_corruption, i));
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
