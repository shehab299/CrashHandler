#include <utils.h>
#include <sys/syscall.h>
#include <dirent.h>
#include <string>
#include <iostream>


void sendSignalToPID(pid_t tid, int signal) {
    pid_t pid = getpid();
    if (syscall(SYS_tgkill, pid, tid, signal) != 0) {
        perror("tgkill failed");
    }
}

int getThreadsCount(pid_t pid)
{
    int count = 0;

    std::string task_dir = "/proc/" + std::to_string(pid) + "/task/";
    DIR* dir = opendir(task_dir.c_str());

    if (!dir) {
        std::cerr << "Failed to open directory: " << task_dir << std::endl;
        return 0;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_name[0] == '.') {
            continue;
        }
        count++;
    }

    closedir(dir);
    return count;
}

std::vector<pid_t> getAllThreadIds(pid_t pid) {
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

