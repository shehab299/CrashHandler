#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
#include "../include/StackTracer.h"


void use_after_free(int thread_id) {
    int* ptr = new int(42);
    if (thread_id == 3) {
        delete ptr;  // Delete the memory
    }
    if (thread_id == 3) {
        std::cout << *ptr << std::endl;  // Access after free, causing use-after-free error
    }
    std::cout << "Thread " << thread_id << " completed\n";
}

int main() {

    StackTracer* instance = StackTracer::getInstance();
    instance->setupStackTracer();


    const int num_threads = 5;  // Number of threads
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.push_back(std::thread(use_after_free, i));
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
