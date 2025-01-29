#ifndef STACK_TRACER_H
#define STACK_TRACER_H

#define BOOST_STACKTRACE_USE_BACKTRACE


#include <vector>
#include <atomic>
#include <pthread.h>
#include <sys/types.h>
#include <mutex>
#include <csignal>

class StackTracer {
public:
    void setupStackTracer();
    
    static StackTracer* getInstance();
    std::vector<int> signals = {SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGBUS};

private:
    StackTracer();
    static void signal_handler(int sig);
    static void print_backtrace();
    int send_signal_to_all_threads(int sig);
    std::vector<pid_t> get_all_thread_ids(pid_t pid);

    std::atomic<bool> first_signal_received;
    pthread_mutex_t log_mutex;
};

#endif // STACK_TRACER_H