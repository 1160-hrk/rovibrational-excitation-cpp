#include "rvx/lvne.hpp"
#include <complex>

namespace rvx {

void rk4_lvne(const Eigen::Ref<const Eigen::MatrixXcd>& H0,
              const Eigen::Ref<const Eigen::MatrixXcd>& mu_x,
              const Eigen::Ref<const Eigen::MatrixXcd>& mu_y,
              const Eigen::Ref<const Eigen::VectorXcd>& Efield_x,
              const Eigen::Ref<const Eigen::VectorXcd>& Efield_y,
              Eigen::Ref<Eigen::MatrixXcd>              rho,
              double                                    dt,
              int                                       steps)
{
    using cplx = std::complex<double>;
    for (int step = 0; step < steps; ++step) {
        const cplx Ex1 = Efield_x(2 * step    );
        const cplx Ey1 = Efield_y(2 * step    );
        const cplx Ex2 = Efield_x(2 * step + 1);
        const cplx Ey2 = Efield_y(2 * step + 1);
        const cplx Ex4 = Efield_x(2 * step + 2);
        const cplx Ey4 = Efield_y(2 * step + 2);

        const Eigen::MatrixXcd H1 = H0 + mu_x * Ex1 + mu_y * Ey1;
        const Eigen::MatrixXcd H2 = H0 + mu_x * Ex2 + mu_y * Ey2;
        const Eigen::MatrixXcd H3 = H2;  // 中点なので H2 と同じ
        const Eigen::MatrixXcd H4 = H0 + mu_x * Ex4 + mu_y * Ey4;

        const Eigen::MatrixXcd k1 = -cplx{0,1} * (H1 * rho - rho * H1);
        const Eigen::MatrixXcd k2 = -cplx{0,1} * (H2 * (rho + 0.5*dt*k1) - (rho + 0.5*dt*k1) * H2);
        const Eigen::MatrixXcd k3 = -cplx{0,1} * (H3 * (rho + 0.5*dt*k2) - (rho + 0.5*dt*k2) * H3);
        const Eigen::MatrixXcd k4 = -cplx{0,1} * (H4 * (rho + dt*k3)    - (rho + dt*k3)    * H4);

        rho += (dt / 6.0) * (k1 + 2.0*k2 + 2.0*k3 + k4);
    }
}

}  // namespace rvx
