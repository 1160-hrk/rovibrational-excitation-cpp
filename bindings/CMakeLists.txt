# bindings/CMakeLists.txt
# ===== Python モジュール =====

# Schrödinger side
pybind11_add_module(psi psi.cpp)
target_link_libraries(psi PRIVATE rvx_core OpenMP::OpenMP_CXX)

# Liouville-von Neumann
pybind11_add_module(lvne lvne.cpp utils.cpp)
target_link_libraries(lvne PRIVATE rvx_core OpenMP::OpenMP_CXX)

target_compile_options(lvne PRIVATE -O3 -fopenmp)
set_target_properties(lvne PROPERTIES POSITION_INDEPENDENT_CODE ON)
