#ifndef STACK_TRACER_H
#define STACK_TRACER_H

#define BOOST_STACKTRACE_USE_BACKTRACE

#include <string>
#include <vector>
#include <atomic>
#include <pthread.h>
#include <sys/types.h>
#include <mutex>
#include <csignal>

class StackTracer {
public:

    enum Output
    {
        STDOUT, 
        STDERR,
        FILE
    };

    void setupStackTracer(Output sink = STDOUT, const std::string& filepath = "");
    static StackTracer* getInstance();
    

    std::vector<int> signals = {SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGBUS};

private:
    StackTracer();
    static void signalHandler(int sig);
    static void printBacktrace();
   
    void setupOutput(Output sink = STDOUT, const std::string& filepath = "");
    void sendSignalToAllThreads(int sig);

    std::atomic<bool> first_signal_received;
    pthread_mutex_t log_mutex;
    int output_descriptor;
    std::string output_path;
};

#endif // STACK_TRACER_H