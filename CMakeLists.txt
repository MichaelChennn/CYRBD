# set vcpkg path
# set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
cmake_minimum_required(VERSION 3.14)

project(RBD)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(EXCUTABLE_NAME rbd)

# find igraph library
find_package(igraph CONFIG REQUIRED)

# find nlohmann_json library
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# include_directories(${IGRAPH_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(rbd_library STATIC src/rbd.cpp src/rbd_utility.cpp src/rbd_bool.cpp)
target_link_libraries(rbd_library igraph::igraph nlohmann_json::nlohmann_json)

# add_executable(rbd src/rbd.cpp)
# add_executable(rbd_bool src/rbd_bool.cpp)
add_executable(unit_test src/unit_test.cpp)

# target_link_libraries(rbd rbd_library)
# target_link_libraries(rbd_bool rbd_library)
target_link_libraries(unit_test rbd_library)
