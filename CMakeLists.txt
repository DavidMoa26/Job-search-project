cmake_minimum_required(VERSION 3.17)
project(Job_Search) # Ensure this matches your CLion project name

set(CMAKE_CXX_STANDARD 14)

# Assuming SQLiteCpp and sqlite3 are not installed globally and are part of the project directory
# Add the SQLiteCpp directory to the build
add_subdirectory(SQLiteCpp)

# Include the SQLiteCpp and SQLite directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp/sqlite3)

# Add sqlite3.c from the SQLiteCpp submodule if not using a precompiled binary
# This assumes sqlite3.c is available in the SQLiteCpp/sqlite3 directory
list(APPEND PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp/sqlite3/sqlite3.c)

# Add your main.cpp and any other source files
list(APPEND PROJECT_SOURCES main.cpp)

# Define the untitled executable
add_executable(Job_Search ${PROJECT_SOURCES})

# Link against SQLiteCpp and sqlite3 (if sqlite3 is not included as part of SQLiteCpp)
target_link_libraries(Job_Search SQLiteCpp)
# If SQLiteCpp is configured to use the internal (or external) SQLite3, you might not need to explicitly link sqlite3 here