# StackTracer Library Documentation

## Overview
StackTracer is a C++ library for capturing stack traces across multi-threaded applications when signals occur. It uses boost::stacktrace for backtrace generation and implements a thread-safe singleton pattern.

## Core Features

### Signal Handling
- Custom signal handling across all application threads
- First-signal propagation to all threads
- Configurable signal set
- Non-blocking signal handler implementation

### Stack Trace Generation
- Detailed backtraces using boost::stacktrace
- Thread-specific stack information
- Function names and locations when available
- Stack depth reporting

### Thread Management
- Automatic thread discovery via /proc filesystem
- Thread-safe logging mechanism
- Cross-thread signal propagation
- Mutex-protected output handling

### Design Patterns
- Singleton pattern for global state management
- Thread-safe implementation using atomic operations
- RAII principles for resource management
- Exception-safe signal handling

## Implementation Details

### Signal Propagation Flow
1. Signal recieved by a thread (The thread that caused the crash or randomly if it is a non-crashing signal)
2. First-signal detection via atomic operation
3. Thread ID collection from /proc
4. Signal propagation to all threads
5. Stack trace capture and logging

### Thread Safety Mechanisms
- Atomic operations for first-signal detection
- Mutex protection for stdout logging
- Thread-safe singleton access
- Safe process/thread ID handling

### Resource Management
- POSIX thread primitives
- File descriptor management for /proc
- Signal handler registration cleanup
- Mutex initialization and cleanup

# User Guide

## Basic Usage

```cpp
#include "StackTracer.h"

int main() {
    auto tracer = StackTracer::getInstance();
    tracer->signals = {SIGSEGV, SIGABRT};
    tracer->setupStackTracer();
    return 0;
}
```

## Configuration Examples

### Monitoring Multiple Signals
```cpp
tracer->signals = {
    SIGSEGV,  // Segmentation fault
    SIGABRT,  // Abort
    SIGFPE,   // Floating point exception
    SIGILL    // Illegal instruction
};
```

### Integration with Existing Signal Handlers
```cpp
struct sigaction existing_handler;
sigaction(SIGSEGV, nullptr, &existing_handler);
tracer->signals = {SIGSEGV};
tracer->setupStackTracer();
```

## Output Example
```
Backtrace for thread 140735272933120:
 0# my_function() at my_file.cpp:123
 1# main at main.cpp:45
```

## Best Practices

### Initialization
- Initialize early in program execution
- Configure signals before setup
- Verify setup success

### Signal Selection
- Choose relevant signals only
- Avoid signals used by debuggers
- Consider platform differences

### Thread Management
- Account for thread creation/destruction
- Handle thread pool scenarios
- Consider thread naming for clarity

### Error Handling
- Check setup return status
- Monitor log output
- Handle /proc access failures