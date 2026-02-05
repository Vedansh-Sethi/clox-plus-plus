cmake_minimum_required(VERSION 3.15)

# Project name and version
project(clox++ VERSION 1.0.0 LANGUAGES CXX)

# Enforce C++17 or C++20 (Recommended for std::variant, std::optional, std::string_view)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Use standard C++, not compiler-specific extensions

# Enable helpful compiler warnings (Essential for compiler dev!)
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Add the source directory
add_subdirectory(src)