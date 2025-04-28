#pragma once
/**
 *  @file lvne.hpp
 *  @brief Liouville–von Neumann 方程式（密度行列）の時間発展 API
 */
#include <Eigen/Core>
#include <complex>
#include "rvx/utils.hpp"

namespace rvx {

/**
 *  4次の Runge–Kutta で密度行列を時間発展させる
 *
 *  @param[in]  H0       基底ハミルトニアン         (dim × dim)
 *  @param[in]  mu_x,y   双極子行列                  (dim × dim)
 *  @param[in]  Efield_* 外場（長さ 2·steps+1 の 1-D 配列）
 *  @param[in]  dt       タイムステップ
 *  @param[in]  steps    ステップ数 (= 時刻点は 2·steps+1)
 *  @param[in,out] rho   密度行列 (dim × dim) — in-place 更新
 */
void rk4_lvne(const Eigen::Ref<const Eigen::MatrixXcd>& H0,
              const Eigen::Ref<const Eigen::MatrixXcd>& mu_x,
              const Eigen::Ref<const Eigen::MatrixXcd>& mu_y,
              const Eigen::Ref<const Eigen::VectorXcd>& Efield_x,
              const Eigen::Ref<const Eigen::VectorXcd>& Efield_y,
              Eigen::Ref<Eigen::MatrixXcd>              rho,
              double                                    dt,
              int                                       steps);

}  // namespace rvx
