#include "../include/StackTracer.h"
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>


// Function to deliberately cause a segmentation fault
void trigger_segfault() {
    int* invalid_ptr = nullptr; // Null pointer
    *invalid_ptr = 42; // Writing to null pointer causes SIGSEGV
}

// Simulate some work in each thread
void do_some_work(int thread_id) {
    sleep(3);

    if (thread_id == 2) { // Let thread 2 trigger the segmentation fault
        std::cout << "Thread " << thread_id << " is about to cause a segmentation fault!\n";
        trigger_segfault();
    }

    do_some_work(thread_id);
}

void* thread_func(void* arg) {
    int thread_id = *static_cast<int*>(arg);
    do_some_work(thread_id);
    sleep(1000); // Simulate work by sleeping indefinitely
    return nullptr;
}

void start_threads(int num_threads) {
    pthread_t tid;
    int thread_id = 0;

    // Create threads
    for (int i = 0; i < num_threads; ++i) {
        pthread_create(&tid, nullptr, thread_func, &thread_id);
        thread_id++; // Increment thread_id for each new thread
    }
}

int main() {
    std::cout << "MY PID: " << getpid() << std::endl;

    // Create a StackTracer instance and setup signals
    StackTracer* stack_tracer = StackTracer::getInstance();
    stack_tracer->setupStackTracer();

    const int num_threads = 5; // Number of threads to spawn

    // Start the threads
    start_threads(num_threads);

    while (true) {
        pause(); // Wait for signals
    }

    return 0;
}
