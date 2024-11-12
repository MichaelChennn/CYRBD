cmake_minimum_required(VERSION 3.14)

project(RBD)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(EXCUTABLE_NAME rbd)

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

add_executable(rbd src/rbd.cpp src/rbd_utility.cpp)
add_executable(rbd_bool src/rbd_bool.cpp src/rbd_utility.cpp)
add_executable(unit_test src/unit_test.cpp src/rbd_utility.cpp src/rbd_bool.cpp src/rbd.cpp)

target_link_libraries(rbd igraph::igraph nlohmann_json::nlohmann_json)
target_link_libraries(rbd_bool igraph::igraph nlohmann_json::nlohmann_json)
target_link_libraries(unit_test igraph::igraph nlohmann_json::nlohmann_json)
