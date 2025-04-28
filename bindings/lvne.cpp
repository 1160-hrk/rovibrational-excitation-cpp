/* bindings/py_propagator.cpp
 * 2025-04 rewrite : new directory layout (rvx:: namespace)
 */
 #include <pybind11/pybind11.h>
 #include <pybind11/eigen.h>
 #include <pybind11/numpy.h>
 #include <rvx/lvne.hpp>                 // ★ 旧 "propagator.hpp" から変更
 
 namespace py = pybind11;
 using Complex = std::complex<double>;
 
 /* ------- numpy <-> Eigen 変換だけを行う薄いラッパ ------- */
 static void rk4_lvne_adapter(
         const py::EigenDRef<const Eigen::MatrixXcd>& H0_in,
         const py::EigenDRef<const Eigen::MatrixXcd>& mu_x_in,
         const py::EigenDRef<const Eigen::MatrixXcd>& mu_y_in,
         py::array_t<Complex, py::array::c_style | py::array::forcecast> Ex_raw,
         py::array_t<Complex, py::array::c_style | py::array::forcecast> Ey_raw,
         py::EigenDRef<Eigen::MatrixXcd> rho_inout,
         double dt, int steps)
 {
     /* ---- 1-D 電場 -> Eigen::VectorXcd ---- */
     auto bx = Ex_raw.request(), by = Ey_raw.request();
     if (bx.ndim != by.ndim || bx.shape != by.shape)
         throw std::runtime_error("Efield_x / Efield_y : shape mismatch");
     if (!(bx.ndim == 1 || (bx.ndim == 2 && bx.shape[1] == 1)))
         throw std::runtime_error("E-field must be 1-D or (N,1)");
 
     Eigen::Map<const Eigen::VectorXcd> Ex(
         static_cast<Complex*>(bx.ptr), bx.shape[0]);
     Eigen::Map<const Eigen::VectorXcd> Ey(
         static_cast<Complex*>(by.ptr), by.shape[0]);
 
     /* ---- 任意ストライド -> 連続領域にコピー ---- */
     Eigen::MatrixXcd H0   = H0_in;
     Eigen::MatrixXcd mu_x = mu_x_in;
     Eigen::MatrixXcd mu_y = mu_y_in;
     Eigen::MatrixXcd rho  = rho_inout;          // copy-in
 
     /* ---- 本体呼び出し ★ new name ★ ---- */
     rvx::rk4_lvne(H0, mu_x, mu_y, Ex, Ey, rho, dt, steps);
 
     /* ---- Python 側行列へ書き戻し ---- */
     rho_inout = rho;
 }
 
 /* ------------ pybind11 module ------------ */
 PYBIND11_MODULE(fast_propagator, m)
 {
     m.doc() = "Fast rovibrational propagator (Liouville-von Neumann RK4)";
 
     /* Python からは rk4_lvne() で呼べるようにする */
     m.def("rk4_lvne", &rk4_lvne_adapter,
           py::arg("H0"), py::arg("mu_x"), py::arg("mu_y"),
           py::arg("Efield_x"), py::arg("Efield_y"),
           py::arg("rho0"), py::arg("dt"), py::arg("steps"),
           R"pbdoc(
               4th-order Runge–Kutta propagator for the Liouville–von Neumann equation.
               `Efield_*` can be 1-D (N,) or (N,1).
           )pbdoc");
 
     /* 便利ユーティリティ（そのまま転送） */
     m.def("initialize_eigen_threads", &initialize_eigen_threads);
     m.def("get_eigen_num_threads",   &get_eigen_num_threads);
     m.def("set_eigen_num_threads",   &set_eigen_num_threads, py::arg("n"));
 }
 