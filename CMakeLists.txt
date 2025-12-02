cmake_minimum_required(VERSION 3.10)
include(FetchContent)
project(price)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python3 interpreter and development libraries
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)


FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.14.1
)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)
add_executable(price main.cpp include/convert.cpp)

# Link libraries
target_link_libraries(price PRIVATE 
    cpr::cpr 
    nlohmann_json::nlohmann_json
    Python3::Python
)

target_include_directories(price PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Include Python headers
target_include_directories(price PRIVATE ${Python3_INCLUDE_DIRS})
