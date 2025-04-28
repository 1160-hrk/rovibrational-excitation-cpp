# bindings/CMakeLists.txt
# ===== Python モジュール =====
pybind11_add_module(lvne lvne.cpp)

target_link_libraries(lvne
    PRIVATE
        rvx_core         # 数値計算ライブラリ
        OpenMP::OpenMP_CXX)

target_compile_options(lvne PRIVATE -O3 -fopenmp)
set_target_properties(lvne PROPERTIES POSITION_INDEPENDENT_CODE ON)
