#pragma once
#include <Eigen/Core>
#include <vector>

namespace rvx {

/* ── 1 回だけ計算 ───────────────────────────────
 * psi_list : N_state 個の列ベクトル | size()==dim
 * H0, mu_x, mu_y : (dim × dim)
 * Efield_*      : 長さ 2*steps+1
 */
void rk4_psi(
    std::vector<Eigen::VectorXcd>&   psi_list,   // in-out
    const Eigen::Ref<const Eigen::MatrixXcd>& H0,
    const Eigen::Ref<const Eigen::MatrixXcd>& mu_x,
    const Eigen::Ref<const Eigen::MatrixXcd>& mu_y,
    const Eigen::Ref<const Eigen::VectorXcd>& Efield_x,
    const Eigen::Ref<const Eigen::VectorXcd>& Efield_y,
    double dt,
    int    steps);

/* ── 軌跡を保存 ────────────────────────────────
 * sample_stride が 10 なら 0,10,20,…step ごとに保存
 *   戻り値: traj[state_i][sample_j] = ψ(dim)
 */
std::vector<std::vector<Eigen::VectorXcd>>
rk4_psi_traj(
    const std::vector<Eigen::VectorXcd>& psi_init,
    const Eigen::Ref<const Eigen::MatrixXcd>& H0,
    const Eigen::Ref<const Eigen::MatrixXcd>& mu_x,
    const Eigen::Ref<const Eigen::MatrixXcd>& mu_y,
    const Eigen::Ref<const Eigen::VectorXcd>& Efield_x,
    const Eigen::Ref<const Eigen::VectorXcd>& Efield_y,
    double dt,
    int    steps,
    int    sample_stride = 1);

} // namespace rvx
