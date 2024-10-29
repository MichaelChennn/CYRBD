cmake_minimum_required(VERSION 3.14)

project(RBD)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(EXCUTABLE_NAME rbd)

#find igraph library
find_package(igraph REQUIRED)


include_directories(${IGRAPH_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.10.5
)
FetchContent_MakeAvailable(json)

add_executable(${EXCUTABLE_NAME} src/rbd.cpp src/utiliy.cpp)

target_link_libraries(${EXCUTABLE_NAME} igraph::igraph nlohmann_json::nlohmann_json)
