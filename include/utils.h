#include <unistd.h>
#include <vector>

void sendSignalToPID(pid_t tid, int signal);
int getThreadsCount(pid_t pid);
std::vector<pid_t> getAllThreadIds(pid_t pid);
