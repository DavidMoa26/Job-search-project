cmake_minimum_required(VERSION 3.23.2)
project(JobSearchProject)  # Adjust project name as needed

set(CMAKE_CXX_STANDARD 14)

# Add the SQLiteCpp directory to the build
<<<<<<< HEAD
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp)



# Add Google Test
=======
add_subdirectory(SQLiteCpp)

# Add Google Test
add_subdirectory(googletest)
>>>>>>> b33e9a15ab5f96340d8258c7333df68ec46262d4

# Define the executable for your main application
add_executable(
        JobSearchProject  # Adjust executable name as needed
        main.cpp
        authentication.cpp
        menus.cpp
        candidate.cpp
        employer.cpp
        tests/main.cpp
        candidate.h)

# Link against SQLiteCpp (and sqlite3 if necessary)
target_link_libraries(JobSearchProject SQLiteCpp)

# Include directories for the main application
target_include_directories(JobSearchProject PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SQLiteCpp/include)