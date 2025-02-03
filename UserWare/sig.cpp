#include "../include/StackTracer.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <unistd.h>
#include <signal.h>

// Function to deliberately cause a segmentation fault
void trigger_segfault() {
    int* invalid_ptr = nullptr; // Null pointer
    *invalid_ptr = 42; // Writing to null pointer causes SIGSEGV
}

// Simulate some work in each thread
void do_some_work(int thread_id) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "Thread " << thread_id << " is working...\n";

    if (thread_id == 2) { // Let thread 2 trigger the segmentation fault
        std::cout << "Thread " << thread_id << " is about to cause a segmentation fault!\n";
        sleep(10);
        trigger_segfault();
    }

    // Simulate some work (no recursion)
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));  // Simulate work
    }
}

int main() {
    std::cout << "MY PID: " << getpid() << std::endl;

    // Create a StackTracer instance and setup signals
    StackTracer* stack_tracer = StackTracer::getInstance();
    stack_tracer->setupStackTracer(); // Ensure correct arguments

    const int num_threads = 5; // Number of threads to spawn
    std::vector<std::thread> threads;

    // Start threads
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(do_some_work, i);
    }

    // Wait for all threads to finish (in this case, they are stuck in an infinite loop, so the program never exits)
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
