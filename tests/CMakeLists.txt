cmake_minimum_required(VERSION 3.17)
project(JobSearchTests)

# Define the test executable
add_executable(tests
        authentication_test.cpp
        ../authentication.cpp
        ../menus.cpp
        ../candidate.cpp
        ../employer.cpp
        # Add other necessary source files from your project
)

# Adjust the include directories
target_include_directories(tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../ # For your project headers
        # You may not need to manually include SQLiteCpp if it's properly added in the main CMakeLists.txt
        # But ensure the SQLiteCpp and any other necessary include paths are correct and included if needed
)

# Link the test executable against Google Test and SQLiteCpp
target_link_libraries(tests gtest gtest_main SQLiteCpp)
