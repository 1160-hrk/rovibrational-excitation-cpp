#pragma once
/**
 *  @file utils.hpp
 *  @brief  Eigen のスレッド周りなど，rvx 全体で共有するユーティリティ
 */
#include <Eigen/Core>

namespace rvx {

/** 既定で OpenMP のスレッド数を流用して Eigen のスレッドを初期化 */
void initialize_eigen_threads();

/** 現在 Eigen が使おうとしているスレッド数を取得 */
int  get_eigen_num_threads();

/** Eigen に使わせたいスレッド数を設定（0 以下は無視） */
void set_eigen_num_threads(int n);

// 空きメモリ (Byte)。取得できなければ 0
std::size_t avail_mem_bytes();
}  // namespace rvx
