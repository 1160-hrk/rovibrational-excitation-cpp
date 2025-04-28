#pragma once

#include <Eigen/Core>
#include <complex>

namespace rvx {

void rk4_propagate(
    const Eigen::Ref<const Eigen::MatrixXcd>& H0,
    const Eigen::Ref<const Eigen::MatrixXcd>& mu_x,
    const Eigen::Ref<const Eigen::MatrixXcd>& mu_y,
    const Eigen::Ref<const Eigen::VectorXcd>& Efield_x,
    const Eigen::Ref<const Eigen::VectorXcd>& Efield_y,
    Eigen::Ref<Eigen::MatrixXcd> rho,
    double dt,
    int steps
);

}