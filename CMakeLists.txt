
cmake_minimum_required(VERSION 3.14)

project(RBD)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# find and include pybind11
set(pybind11_DIR "/home/chenyaxuan123/.local/lib/python3.8/site-packages/pybind11/share/cmake/pybind11")
find_package(pybind11 REQUIRED)

# find nlohmann_json library
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# set source files
set(SRC_FILES
    src/rbd.cpp
)

# include_directories(${IGRAPH_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# create a static library
add_library(rbd_library STATIC ${SRC_FILES})
# set the -fPIC flag on
set_target_properties(rbd_library PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(rbd_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(rbd_library nlohmann_json::nlohmann_json)

# set up unit test
# add_executable(unit_test src/unit_test.cpp)
# target_link_libraries(unit_test rbd_library)

#set up benchmark
# add_executable(benchmark src/benchmark.cpp)
# target_link_libraries(benchmark rbd_library)

# set up python module
pybind11_add_module(rbd_bindings src/python_bindings.cpp)
target_link_libraries(rbd_bindings PRIVATE rbd_library)