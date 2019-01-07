cmake_minimum_required(VERSION 3.5)

project(std_error_code
    VERSION 0.0.1
    LANGUAGES C CXX
    DESCRIPTION "Example of hooking into std::error_code machinery"
    )

add_executable(std_error_code main.cpp)
set_target_properties(std_error_code PROPERTIES CXX_STANDARD 17)
