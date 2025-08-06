cmake_minimum_required(VERSION 3.10)
project(price)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python3 interpreter and development libraries
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Add external libraries
add_subdirectory(external/cpr)
add_subdirectory(external/json)

add_executable(price main.cpp)

# Link libraries
target_link_libraries(price PRIVATE 
    cpr::cpr 
    nlohmann_json::nlohmann_json
    Python3::Python
)

# Include Python headers
target_include_directories(price PRIVATE ${Python3_INCLUDE_DIRS})
