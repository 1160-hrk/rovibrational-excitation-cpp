#include "rvx/utils.hpp"
#include <Eigen/Core>
#include <omp.h>
#include <unistd.h>   // sysconf

namespace rvx {

/* ==== スレッド数 ==== */
void initialize_eigen_threads() {Eigen::setNbThreads(omp_get_max_threads()); }
int get_eigen_num_threads() {return Eigen::nbThreads(); }
void set_eigen_num_threads(int n) {Eigen::setNbThreads(n); }

/* ==== 空きメモリ（Linux/macOS） ==== */
std::size_t avail_mem_bytes()
{
    long pages = sysconf(_SC_AVPHYS_PAGES);
    long size  = sysconf(_SC_PAGE_SIZE);
    return (pages>0 && size>0) ? std::size_t(pages)*std::size_t(size) : 0;
}

} // namespace rvx
