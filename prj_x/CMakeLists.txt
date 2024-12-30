project(StackTracer)
cmake_minimum_required(VERSION 3.10)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../lib)

# Boost
set(BOOST_ROOT "../Vendors/booost")
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")

# Find dependencies
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system stacktrace_backtrace)


# Include directories
include_directories(${CMAKE_SOURCE_DIR}/../include ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/../Vendors/libbacktrace)

# Source files
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/../source/*.cpp)

# Create library
add_library(StackTracer STATIC ${SOURCES})

# Set target properties
target_link_libraries(StackTracer
    PUBLIC
    Boost::boost  # Boost headers
    Boost::stacktrace_backtrace
    dl
    Threads::Threads
    ${CMAKE_SOURCE_DIR}/../Vendors/libbacktrace/lib/libbacktrace.a
)


target_link_options(StackTracer PUBLIC -Wl,--whole-archive -Wl,--no-whole-archive)
