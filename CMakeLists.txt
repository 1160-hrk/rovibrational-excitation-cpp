cmake_minimum_required(VERSION 3.14)
project(rovibrational_excitation_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# === OpenMP ===
find_package(OpenMP REQUIRED)

# === pybind11 ===
find_package(pybind11 REQUIRED)

# === Eigen3 ===
find_path(EIGEN3_INCLUDE_DIR Eigen/Dense PATH_SUFFIXES eigen3)
if(NOT EIGEN3_INCLUDE_DIR)
    message(FATAL_ERROR "Eigen3 not found!")
endif()

# === propagator ライブラリ ===
add_library(propagator STATIC src/propagator.cpp)
target_include_directories(propagator PUBLIC include ${EIGEN3_INCLUDE_DIR})
target_link_libraries(propagator PUBLIC OpenMP::OpenMP_CXX)
target_compile_options(propagator PRIVATE -O3 -fopenmp)  # ← 最適化＆OpenMP

# === pybind11 Python モジュール ===
pybind11_add_module(fast_propagator bindings/py_propagator.cpp)
target_link_libraries(fast_propagator PRIVATE propagator OpenMP::OpenMP_CXX)
target_compile_options(fast_propagator PRIVATE -O3 -fopenmp)
set_target_properties(fast_propagator PROPERTIES POSITION_INDEPENDENT_CODE ON)

# === install ===
install(TARGETS fast_propagator DESTINATION .)

# === メッセージ ===
message(STATUS "Eigen3 include dir: ${EIGEN3_INCLUDE_DIR}")
message(STATUS "OpenMP found: ${OpenMP_CXX_FOUND}")
