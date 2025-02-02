cmake_minimum_required(VERSION 3.10)
project(StackTracer)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../lib)

# Boost
set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/../Vendors/boost")
set(BACKTRACE_ROOT "${CMAKE_SOURCE_DIR}/../Vendors/libbacktrace")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
set(LIBBACKTRACE_INCLUDEDIR "${BACKTRACE_ROOT}/include")
set(LIBBACKTRACE_LIBRARYDIR "${BACKTRACE_ROOT}/lib")


# Find dependencies
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system stacktrace_backtrace)


# Include directories
include_directories(${CMAKE_SOURCE_DIR}/../include ${Boost_INCLUDE_DIRS} ${LIBBACKTRACE_INCLUDEDIR})

# Source files
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/../source/*.cpp)

set(LIBBACKTRACE_ARCHIVES "${BACKTRACE_ROOT}/lib/libbacktrace.a")

foreach(LIBBACKTRACE_ARCHIVE ${LIBBACKTRACE_ARCHIVES})
    if(EXISTS ${LIBBACKTRACE_ARCHIVE})
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/libbacktrace_extracted
        )
        execute_process(
            COMMAND ar x ${LIBBACKTRACE_ARCHIVE}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/libbacktrace_extracted
        )
    endif()
endforeach()

file(GLOB_RECURSE BACKTRACE_OBJECTS ${CMAKE_BINARY_DIR}/libbacktrace_extracted/*.o)

# Create library
add_library(StackTracer STATIC ${SOURCES} ${BACKTRACE_OBJECTS})

# Set target properties
target_link_libraries(StackTracer
    PUBLIC
    Boost::boost 
    Boost::stacktrace_backtrace
    dl
    Threads::Threads
)


# target_link_options(StackTracer PUBLIC -Wl,--whole-archive -Wl,--no-whole-archive)
