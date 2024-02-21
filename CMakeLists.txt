cmake_minimum_required(VERSION 3.17)
project(Job_Search) # Ensure this matches your CLion project name

set(CMAKE_CXX_STANDARD 14)

# Add the SQLiteCpp directory to the build
add_subdirectory(SQLiteCpp)

# Include the SQLiteCpp and SQLite directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp/include)

# Add sqlite3.c from the SQLiteCpp submodule if not using a precompiled binary
# This assumes sqlite3.c is available in the SQLiteCpp/sqlite3 directory
list(APPEND PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp/sqlite3/sqlite3.c)

# Add your main.cpp and any other source files
list(APPEND PROJECT_SOURCES main.cpp)

# Define the untitled executable
add_executable(Job_Search ${PROJECT_SOURCES})

# Link against SQLiteCpp
target_link_libraries(Job_Search SQLiteCpp)
