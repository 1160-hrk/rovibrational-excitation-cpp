#include "rvx/psi.hpp"
#include "rvx/utils.hpp"
#include <complex>
#include <sstream>

namespace rvx {

/* ---------- 内部: 1 ステップ ---------- */
static Eigen::VectorXcd rk4_step(
            const Eigen::Ref<const Eigen::VectorXcd>& psi,
            const Eigen::MatrixXcd& H,
            double dt)
{
    using namespace std::complex_literals;
    const auto i  = 1i;

    Eigen::VectorXcd k1 = -i * (H * psi);
    Eigen::VectorXcd k2 = -i * (H * (psi + 0.5*dt*k1));
    Eigen::VectorXcd k3 = -i * (H * (psi + 0.5*dt*k2));
    Eigen::VectorXcd k4 = -i * (H * (psi + dt*k3));

    return psi + dt/6.0 * (k1 + 2.0*k2 + 2.0*k3 + k4);
}

/* ---------- ψ を直接更新 ---------- */
void rk4_psi(
    std::vector<Eigen::VectorXcd>& psi_list,
    const Eigen::Ref<const Eigen::MatrixXcd>& H0,
    const Eigen::Ref<const Eigen::MatrixXcd>& mu_x,
    const Eigen::Ref<const Eigen::MatrixXcd>& mu_y,
    const Eigen::Ref<const Eigen::VectorXcd>& Ex,
    const Eigen::Ref<const Eigen::VectorXcd>& Ey,
    double dt,
    int    steps)
{
    for (int step=0; step<steps; ++step)
    {
        const auto Ex1 = Ex(2*step    ), Ey1 = Ey(2*step    );
        const auto Ex2 = Ex(2*step + 1), Ey2 = Ey(2*step + 1);
        const auto Ex4 = Ex(2*step + 2), Ey4 = Ey(2*step + 2);

        const Eigen::MatrixXcd H1 = H0 + mu_x*Ex1 + mu_y*Ey1;
        const Eigen::MatrixXcd H2 = H0 + mu_x*Ex2 + mu_y*Ey2;
        const Eigen::MatrixXcd H4 = H0 + mu_x*Ex4 + mu_y*Ey4;

        for (auto& psi : psi_list)
        {
            // k2, k3 用に midpoint Hamiltonian (H2) を使うので
            psi = rk4_step(rk4_step(rk4_step(psi, H1, dt/2), H2, dt/2), H4, dt);
        }
    }
}

/* ---------- 軌跡付き ---------- */
std::vector<std::vector<Eigen::VectorXcd>>
rk4_psi_traj(
    const std::vector<Eigen::VectorXcd>& psi_init,
    const Eigen::Ref<const Eigen::MatrixXcd>& H0,
    const Eigen::Ref<const Eigen::MatrixXcd>& mu_x,
    const Eigen::Ref<const Eigen::MatrixXcd>& mu_y,
    const Eigen::Ref<const Eigen::VectorXcd>& Ex,
    const Eigen::Ref<const Eigen::VectorXcd>& Ey,
    double dt,
    int    steps,
    int    sample_stride)
{
    if (sample_stride < 1)
        throw std::invalid_argument("sample_stride must be >=1");

    const std::size_t n_states = psi_init.size();
    const int dim   = psi_init[0].size();
    const int n_samples = steps / sample_stride + 1;

    /* === メモリ試算 & チェック === */
    const std::size_t bytes_needed =
        n_states * std::size_t(n_samples) * dim * sizeof(std::complex<double>);
    std::size_t bytes_free = avail_mem_bytes();
    if (bytes_free && bytes_needed > bytes_free) {
        std::ostringstream oss;
        oss << "[rvx::rk4_psi_traj] requires "
            << (bytes_needed>>20) << " MiB but only "
            << (bytes_free >>20)  << " MiB free -- "
            << "increase sample_stride";
        throw std::runtime_error(oss.str());
    }

    /* === メモリ確保 === */
    std::vector<std::vector<Eigen::VectorXcd>>
        traj(n_states, std::vector<Eigen::VectorXcd>(n_samples));

    std::vector<Eigen::VectorXcd> psi = psi_init;   // 作業用コピー
    int sample_idx = 0;
    for (std::size_t s=0; s<n_states; ++s) traj[s][0] = psi[s]; // t=0

    /* === メインループ === */
    for (int step=1; step<=steps; ++step)
    {
        rk4_psi(psi, H0, mu_x, mu_y, Ex, Ey, dt, 1);

        if (step % sample_stride == 0) {
            ++sample_idx;
            for (std::size_t s=0; s<n_states; ++s) traj[s][sample_idx] = psi[s];
        }
    }
    return traj;
}

} // namespace rvx
