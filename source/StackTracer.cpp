#include <StackTracer.h>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <boost/stacktrace.hpp>
#include <csignal>
#include <string>
#include <atomic>
#include <sys/syscall.h>
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

void send_signal_to_tid(pid_t tid, int signal) {
    pid_t pid = getpid();
    if (syscall(SYS_tgkill, pid, tid, signal) != 0) {
        perror("tgkill failed");
    }
}

StackTracer::StackTracer() : first_signal_received(false) {
    log_mutex = PTHREAD_MUTEX_INITIALIZER;
}

void StackTracer::setupStackTracer() {
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    
    for (int signal : signals) {
        sa.sa_handler = &StackTracer::signal_handler;
        sigaction(signal, &sa, nullptr);
    }
}

void StackTracer::signal_handler(int sig) {
    StackTracer* instance = getInstance();
    bool expected = false;
    if (instance->first_signal_received.compare_exchange_strong(expected, true)) {
        
        int nThreads = instance->send_signal_to_all_threads(sig);

        std::unique_lock<std::mutex> lock(cv_mutex);
        cv.wait(lock, [&]() {return completed_threads.load() == nThreads - 1;});

        print_backtrace();

        exit(sig);
        return;
    }


    pthread_mutex_lock(&getInstance()->log_mutex);
    print_backtrace();
    pthread_mutex_unlock(&getInstance()->log_mutex);

    {
        std::lock_guard<std::mutex> lock(cv_mutex);
        completed_threads.fetch_add(1);
    }

    cv.notify_all();

    while(true);
}

void StackTracer::print_backtrace() {
    boost::stacktrace::stacktrace st;
    std::ostringstream oss;
    oss << "Backtrace for thread " << pthread_self() << ":\n" << st << std::endl;

    std::string output = oss.str();
    write(STDOUT_FILENO, output.c_str(), output.size());
}


int StackTracer::send_signal_to_all_threads(int sig) {
    pid_t pid = getpid();
    std::vector<pid_t> thread_ids = get_all_thread_ids(pid);
    for (pid_t tid : thread_ids) {
        send_signal_to_tid(tid, sig);
    }

    return thread_ids.size();
}

std::vector<pid_t> StackTracer::get_all_thread_ids(pid_t pid) {
    std::vector<pid_t> thread_ids;
    std::string task_dir = "/proc/" + std::to_string(pid) + "/task/";
    DIR* dir = opendir(task_dir.c_str());
    
    if (!dir) {
        std::cerr << "Failed to open directory: " << task_dir << std::endl;
        return thread_ids;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_name[0] == '.') {
            continue;
        }
        pid_t tid = std::stoi(entry->d_name);
        thread_ids.push_back(tid);
    }
    closedir(dir);
    return thread_ids;
}

StackTracer* StackTracer::getInstance() {
    static StackTracer instance;
    return &instance;
}

