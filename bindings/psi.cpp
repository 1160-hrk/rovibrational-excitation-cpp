#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include "rvx/psi.hpp"
#include "rvx/utils.hpp"

namespace py = pybind11;
using rvx::rk4_psi;
using rvx::rk4_psi_traj;

PYBIND11_MODULE(psi, m)
{
    m.doc() = "rvx - Schrödinger solvers";

    m.def("rk4_psi",  &rk4_psi,
        py::arg("psi_list"), py::arg("H0"), py::arg("mu_x"), py::arg("mu_y"),
        py::arg("Efield_x"), py::arg("Efield_y"),
        py::arg("dt"), py::arg("steps"));

    m.def("rk4_psi_traj", &rk4_psi_traj,
        py::arg("psi_list"), py::arg("H0"), py::arg("mu_x"), py::arg("mu_y"),
        py::arg("Efield_x"), py::arg("Efield_y"),
        py::arg("dt"), py::arg("steps"), py::arg("sample_stride") = 1);

    // utils
    m.def("initialize_eigen_threads", &rvx::initialize_eigen_threads);
    m.def("get_eigen_num_threads",    &rvx::get_eigen_num_threads);
    m.def("set_eigen_num_threads",    &rvx::set_eigen_num_threads, py::arg("n"));
}
