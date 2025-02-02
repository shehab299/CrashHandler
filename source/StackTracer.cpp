#include <StackTracer.h>
#include <utils.h>

#include <boost/stacktrace.hpp>

#include <iostream>
#include <unistd.h>
#include <csignal>
#include <string>
#include <atomic>
#include <sstream>
#include <string>
#include <cerrno>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <pthread.h>

std::mutex cv_mutex;
std::condition_variable cv;
std::atomic<int> completed_threads(0);

StackTracer::StackTracer() : first_signal_received(false) {
    log_mutex = PTHREAD_MUTEX_INITIALIZER;
}

void StackTracer::setupStackTracer(Output sink, const std::string& filepath) 
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    
    for (int signal : signals) {
        sa.sa_handler = &StackTracer::signalHandler;
        sigaction(signal, &sa, nullptr);
    }

    setupOutput(sink, filepath);
}

void StackTracer::signalHandler(int sig) {
    
    StackTracer* instance = getInstance();
    bool expected = false;
    
    if (instance->first_signal_received.compare_exchange_strong(expected, true)) 
    {
        printBacktrace();        
        instance->sendSignalToAllThreads(sig);

        std::unique_lock<std::mutex> lock(cv_mutex);

        cv.wait(lock, [&]() {
            int nThreads = getThreadsCount(getpid());
            return completed_threads.load() >= nThreads - 1;}
        );

        exit(sig);
        return;
    }


    pthread_mutex_lock(&getInstance()->log_mutex);
    printBacktrace();
    pthread_mutex_unlock(&getInstance()->log_mutex);

    {
        std::lock_guard<std::mutex> lock(cv_mutex);
        completed_threads.fetch_add(1);
    }

    cv.notify_all();

    while(true);
}

void StackTracer::printBacktrace() {

    StackTracer* inst = getInstance();

    if(inst->output_path != "")
    {
        int fd = open(inst->output_path.c_str(), O_WRONLY | O_CREAT | O_APPEND,  0644);

        if(fd == -1)
        {
            std::string errormsg = "Couldn't open " + inst->output_path + " For logging stacktrace";
            write(STDERR_FILENO, errormsg.c_str(), errormsg.size());
            return;
        }

        inst->output_descriptor = fd;
    }

    boost::stacktrace::stacktrace st;
    std::ostringstream oss;
    oss << "Backtrace for thread " << pthread_self() << ":\n" << st << std::endl;

    std::string output = oss.str();
    write(inst->output_descriptor, output.c_str(), output.size());
    close(inst->output_descriptor);
}

void StackTracer::sendSignalToAllThreads(int sig) {
    pid_t pid = getpid();
    std::vector<pid_t> thread_ids = getAllThreadIds(pid);
    for (pid_t tid : thread_ids) {
        sendSignalToPID(tid, sig);
    }
}

StackTracer* StackTracer::getInstance() {
    static StackTracer instance;
    return &instance;
}

void StackTracer::setupOutput(Output sink, const std::string& filepath)
{
    if(sink == Output::STDOUT)
        output_descriptor = STDOUT_FILENO;
    else if(sink == Output::STDERR)
        output_descriptor = STDERR_FILENO;
    else if(sink == Output::FILE)
        output_path = filepath;
}
