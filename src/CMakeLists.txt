# src/CMakeLists.txt
# ===== rvx_core ライブラリ =====
file(GLOB RVX_SOURCES
     "${CMAKE_CURRENT_SOURCE_DIR}/rvx/lvne_rk4.cpp"   # リウヴィル方程式の時間発展
     "${CMAKE_CURRENT_SOURCE_DIR}/rvx/psi_rk4.cpp"   # シュレディンガー方程式の時間発展
     "${CMAKE_CURRENT_SOURCE_DIR}/rvx/utils.cpp"  # 共通ユーティリティ
)
     
add_library(rvx_core STATIC ${RVX_SOURCES})

target_include_directories(rvx_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include         # 公開ヘッダ
        ${EIGEN3_INCLUDE_DIR})               # Eigen
target_link_libraries(rvx_core PUBLIC OpenMP::OpenMP_CXX)
target_compile_options(rvx_core PRIVATE -O3 -fopenmp)

