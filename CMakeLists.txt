# ──────────────────────────────────────────────
#  rvx  (root)  CMakeLists.txt
# ──────────────────────────────────────────────
cmake_minimum_required(VERSION 3.24)
project(rvx LANGUAGES CXX)

# -------- 共通コンパイル設定 ----------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# -------- 外部依存 (全サブプロジェクトで共通) ----------
find_package(OpenMP   REQUIRED)       # 並列化
find_package(pybind11 REQUIRED)       # Python ラッパ
find_path(EIGEN3_INCLUDE_DIR Eigen/Dense PATH_SUFFIXES eigen3)
if (NOT EIGEN3_INCLUDE_DIR)
    message(FATAL_ERROR "Eigen3 not found!")
endif()

# 全ターゲットに公開するインクルードを登録
set(RVX_PUBLIC_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/include
    ${EIGEN3_INCLUDE_DIR}
)

# ---- サブディレクトリ ----
add_subdirectory(src)        # lib rvx         (C++ コア)
add_subdirectory(bindings)   # py modules ...  (pybind11)

# ---- 便利メッセージ ----
message(STATUS "Eigen3 include dir : ${EIGEN3_INCLUDE_DIR}")
message(STATUS "OpenMP found       : ${OpenMP_CXX_FOUND}")
