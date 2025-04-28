#include "rvx/lvne.hpp"
#include <Eigen/Core>
#include <omp.h>

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
) {
    for (int step = 0; step < steps; ++step) {
        auto Ex1 = Efield_x(2 * step);
        auto Ey1 = Efield_y(2 * step);
        auto Ex2 = Efield_x(2 * step + 1);
        auto Ey2 = Efield_y(2 * step + 1);
        auto Ex4 = Efield_x(2 * step + 2);
        auto Ey4 = Efield_y(2 * step + 2);

        Eigen::MatrixXcd H1 = H0 + mu_x * Ex1 + mu_y * Ey1;
        Eigen::MatrixXcd H2 = H0 + mu_x * Ex2 + mu_y * Ey2;
        Eigen::MatrixXcd H3 = H2;
        Eigen::MatrixXcd H4 = H0 + mu_x * Ex4 + mu_y * Ey4;

        Eigen::MatrixXcd k1 = -std::complex<double>(0, 1) * (H1 * rho - rho * H1);
        Eigen::MatrixXcd k2 = -std::complex<double>(0, 1) * (H2 * (rho + 0.5 * dt * k1) - (rho + 0.5 * dt * k1) * H2);
        Eigen::MatrixXcd k3 = -std::complex<double>(0, 1) * (H3 * (rho + 0.5 * dt * k2) - (rho + 0.5 * dt * k2) * H3);
        Eigen::MatrixXcd k4 = -std::complex<double>(0, 1) * (H4 * (rho + dt * k3) - (rho + dt * k3) * H4);

        rho += dt / 6.0 * (k1 + 2.0 * k2 + 2.0 * k3 + k4);
    }
}

}